//! ELightController – MIDI USB → DMX512 via MAX485 on STM32F446RE
//!
//! Hardware connections:
//!   - USB OTG FS (PA11/PA12) ← PC sends MIDI
//!   - USART1 TX (PA9/D8) → MAX485 DI
//!   - PA10 (D2) → MAX485 DE/RE
//!
//! The firmware:
//!   1. Initialises USB as a MIDI device
//!   2. Polls for incoming MIDI messages
//!   3. Maps MIDI CC / Note messages to DMX channel values
//!   4. Periodically transmits a DMX512 frame via USART1 + MAX485

#![no_std]
#![no_main]

use panic_halt as _;

use cortex_m_rt::entry;
use stm32f4xx_hal::{
    gpio::GpioExt,
    otg_fs::{UsbBus, USB},
    pac,
    prelude::*,
    rcc::{Config, RccExt},
    serial,
};
use usb_device::prelude::*;
use usbd_midi::midi_device::MidiClass;

mod constants;
mod dmx;
mod mapping;
mod midi;

use dmx::DmxTransmitter;
use constants::*;

/// USB endpoint memory (must live in static, used by the USB peripheral).
static mut USB_EP_MEMORY: [u32; 1024] = [0; 1024];

#[entry]
fn main() -> ! {
    let dp = pac::Peripherals::take().unwrap();
    let cp = cortex_m::peripheral::Peripherals::take().unwrap();

    // -----------------------------------------------------------------------
    // Clock configuration
    // -----------------------------------------------------------------------
    let mut rcc = dp.RCC.freeze(
        Config::hse(8.MHz())
            .sysclk(84.MHz())
            .require_pll48clk(),
    );

    let mut delay = cp.SYST.delay(&rcc.clocks);

    // -----------------------------------------------------------------------
    // GPIO
    // -----------------------------------------------------------------------
    let gpioa = dp.GPIOA.split(&mut rcc);

    // -- MAX485 DE/RE pin: PA10 on Nucleo-F446RE.
    let de_pin = gpioa.pa10.into_push_pull_output();

    // -- USART1 TX-only for DMX output (TX = PA9/D8, baud = 250000, 8N2)
    let tx_pin = gpioa.pa9.into_alternate();

    let serial_cfg = serial::config::Config::default()
        .baudrate(250_000.bps())
        .wordlength_8()
        .stopbits(serial::config::StopBits::STOP2)
        .parity_none();

    let tx = dp.USART1.tx(tx_pin, serial_cfg, &mut rcc).unwrap();

    let mut dmx = DmxTransmitter::new(tx, de_pin);

    // -----------------------------------------------------------------------
    // USB OTG FS – MIDI device
    // -----------------------------------------------------------------------
    let usb = USB::new(
        (dp.OTG_FS_GLOBAL, dp.OTG_FS_DEVICE, dp.OTG_FS_PWRCLK),
        (gpioa.pa11.into_alternate(), gpioa.pa12.into_alternate()),
        &rcc.clocks,
    );

    let usb_bus = unsafe { UsbBus::new(usb, &mut USB_EP_MEMORY) };

    let mut midi_class = MidiClass::new(&usb_bus, 1, 1).unwrap();
    let mut usb_dev = UsbDeviceBuilder::new(&usb_bus, UsbVidPid(0x1209, 0x0001))
        .strings(&[StringDescriptors::default()
            .manufacturer("EDrums")
            .product("ELightController")
            .serial_number("0001")])
        .unwrap()
        .device_class(0x00)
        .build();

    // -----------------------------------------------------------------------
    // Startup test pattern: cycle R → G → B → W → all off
    // -----------------------------------------------------------------------
    for &(r, g, b, w) in &[
        (255, 0, 0, 0),   // Red
        (0, 255, 0, 0),   // Green
        (0, 0, 255, 0),   // Blue
        (0, 0, 0, 255),   // White
        (255, 255, 255, 255), // All on
        (0, 0, 0, 0),     // Off
    ] {
        dmx.set_channel(CH_RED, r);
        dmx.set_channel(CH_GREEN, g);
        dmx.set_channel(CH_BLUE, b);
        dmx.set_channel(CH_WHITE, w);
        for _ in 0..20 {
            dmx.send_frame(&mut delay);
            delay.delay_ms(25u32);
        }
    }

    // -----------------------------------------------------------------------
    // Main loop
    // -----------------------------------------------------------------------
    let mut frame_counter: u16 = 0;

    loop {
        // Poll USB – must be called frequently
        if usb_dev.poll(&mut [&mut midi_class]) {
            // Try to read a USB-MIDI packet (4 bytes)
            let mut buf = [0u8; 64];
            if let Ok(count) = midi_class.read(&mut buf) {
                // Process complete 4-byte USB-MIDI packets
                let mut i = 0;
                while i + 4 <= count {
                    let packet: [u8; 4] = [buf[i], buf[i + 1], buf[i + 2], buf[i + 3]];
                    if let Some(msg) = midi::parse_usb_midi_packet(&packet) {
                        mapping::apply_midi_to_dmx(msg, &mut dmx);
                    }
                    i += 4;
                }
            }
        }

        // Send a DMX frame every ~25 ms (~40 fps, well within DMX spec)
        frame_counter = frame_counter.wrapping_add(1);
        if frame_counter >= 250 {
            frame_counter = 0;
            dmx.send_frame(&mut delay);
        }
    }
}
