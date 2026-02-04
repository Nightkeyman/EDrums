//! Demonstrate the use of a blocking `Delay` using the SYST (sysclock) timer.

#![deny(unsafe_code)]
#![allow(clippy::empty_loop)]
#![no_main]
#![no_std]

// Halt on panic
use panic_halt as _; // panic handler

use hal::{pac, prelude::*};
use cortex_m_rt::entry;
use stm32f4xx_hal::{self as hal, rcc::Config};

mod constants;
mod utils {
    pub mod calculation_helpers;
    pub mod debouncer;
    pub mod iir_filter;
}
mod drum_controller;
mod drum_element;
mod hal_adapter;

pub trait MidiOut {
    fn send(&mut self, command: u8, note: u8, velocity: u8);
}

use crate::constants::*;
use crate::drum_controller::DrumController;
use crate::drum_element::binary::BinaryDrumElement;
use crate::drum_element::continuous::ContinuousDrumElement;
use crate::drum_element::continuous_binarizer::ContinuousDrumBinarizer;
use crate::drum_element::DrumElement;

use hal_adapter::DummyMidi;

#[entry]
fn main() -> ! {
    if let (Some(dp), Some(cp)) = (
        pac::Peripherals::take(),
        cortex_m::peripheral::Peripherals::take(),
    ) {
        // Set up the system clock. We want to run at 48MHz for this one.
        let rcc = dp.RCC.freeze(Config::hsi().sysclk(48.MHz()));

        // Create a delay abstraction based on SysTick
        let mut delay = cp.SYST.delay(&rcc.clocks);

        // Hardware init (platform-specific) would go here.
        let mut midi = DummyMidi;

        // Create drum elements mirroring original Arduino sketch
        let mut crash_choke = BinaryDrumElement::new(1u8, CHOKE_SIGNAL);
        let mut crash_cymbal =
            ContinuousDrumElement::new(CRASH_MID_VAL, CRASH_SIGNAL, DEFAULT_IDLE_OFFSET);
        let _kick_drum =
            ContinuousDrumBinarizer::new(KICK_IDLE_VALUE, KICK_MIDI_SIGNAL, DEFAULT_IDLE_OFFSET);

        let mut controller = DrumController::new();
        controller.init();

        loop {
            // CRASH CYMBAL
            let crash_hit = hal_adapter::analog_read(CRASH_PIN);
            crash_cymbal.update_state(crash_hit, &mut midi);

            // CHOKE
            let choke_val = hal_adapter::digital_read(CHOKE_PIN);
            crash_choke.update_state(choke_val, &mut midi);

            // Kick (example)
            // let kick_hit = hal_adapter::analog_read(CRASH_PIN);
            // kick_drum.update_state(kick_hit, &mut midi);

            controller.cycle();

            // Small delay to avoid a tight busy loop on real hardware
            delay.delay_ms(1u32);
        }
    }

    loop {}
}
