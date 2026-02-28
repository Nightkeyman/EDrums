/// DMX512 transmitter over UART + MAX485 (DE pin for driver-enable).
///
/// Protocol summary:
///   1. Pull TX low for ≥88 µs  (BREAK)
///   2. Pull TX high for ≥8 µs  (Mark After Break)
///   3. Send start code (0x00) + up to 512 channel bytes at 250 kbaud, 8N2
///
/// Hardware wiring (directly stated by the user):
///   STM PA9 (USART1 TX / D8) → MAX485 DI
///   STM PA10 (D2)              → MAX485 DE/RE

use stm32f4xx_hal::{
    pac::USART1,
    prelude::*,
    serial::Tx,
};

use crate::constants::*;

/// Holds everything needed to send a DMX universe.
pub struct DmxTransmitter<DE> {
    tx: Tx<USART1>,
    de_pin: DE,
    channels: [u8; DMX_MAX_CHANNELS],
}

impl<DE> DmxTransmitter<DE>
where
    DE: embedded_hal::digital::OutputPin,
{
    pub fn new(tx: Tx<USART1>, de_pin: DE) -> Self {
        Self {
            tx,
            de_pin,
            channels: [0u8; DMX_MAX_CHANNELS],
        }
    }

    /// Set a single DMX channel value (1-indexed, as per DMX convention).
    /// Channel 0 is the start code and is not user-settable.
    pub fn set_channel(&mut self, channel: usize, value: u8) {
        if channel >= 1 && channel <= DMX_MAX_CHANNELS {
            self.channels[channel - 1] = value;
        }
    }

    /// Get current value of a DMX channel (1-indexed).
    pub fn get_channel(&self, channel: usize) -> u8 {
        if channel >= 1 && channel <= DMX_MAX_CHANNELS {
            self.channels[channel - 1]
        } else {
            0
        }
    }

    /// Transmit one full DMX frame (break + MAB + start code + channel data).
    ///
    /// This is a blocking call – it ties up the CPU for the duration of
    /// the frame, which is fine for a simple controller.
    pub fn send_frame(&mut self, delay: &mut impl embedded_hal::delay::DelayNs) {
        // Enable MAX485 driver
        let _ = self.de_pin.set_high();

        // --- BREAK + MAB via GPIO bitbang on PA9 (USART1 TX) ---
        // SBK only generates ~40 µs at 250 kbaud, DMX needs ≥ 88 µs.
        // So we temporarily switch PA9 from UART to GPIO, pull low, then back.
        send_break_and_mab(delay);

        // --- Start code ---
        block_write_byte(&mut self.tx, DMX_START_CODE);

        // --- Channel data ---
        for &val in &self.channels {
            block_write_byte(&mut self.tx, val);
        }

        // Wait for last byte to finish transmitting
        block_flush(&mut self.tx);

        // Wait for Transfer Complete (all bits physically shifted out)
        unsafe {
            let usart1 = &*stm32f4xx_hal::pac::USART1::ptr();
            while usart1.sr().read().tc().bit_is_clear() {}
        }

        // Disable driver (go back to receive / high-Z)
        let _ = self.de_pin.set_low();

        // Mark Between Packets — idle time before next BREAK
        delay.delay_us(50u32);
    }
}

// ---------------------------------------------------------------------------
// Low-level helpers
// ---------------------------------------------------------------------------

/// Generate DMX512 BREAK (≥88 µs low) + MAB (≥8 µs high) by temporarily
/// switching PA9 from USART1_TX alternate function to GPIO output.
fn send_break_and_mab(delay: &mut impl embedded_hal::delay::DelayNs) {
    unsafe {
        let usart1 = &*stm32f4xx_hal::pac::USART1::ptr();
        let gpioa = &*stm32f4xx_hal::pac::GPIOA::ptr();

        // Wait for any ongoing transmission to complete (TC = Transfer Complete)
        while usart1.sr().read().tc().bit_is_clear() {}

        // Disable USART transmitter
        usart1.cr1().modify(|_, w| w.te().clear_bit());

        // Switch PA9 from alternate function (MODER=10) to general output (MODER=01)
        // PA9 = bits [19:18] in MODER
        gpioa.moder().modify(|r, w| {
            w.bits((r.bits() & !(0b11 << 18)) | (0b01 << 18))
        });

        // Drive PA9 LOW → BREAK
        gpioa.bsrr().write(|w| w.br9().set_bit());
        delay.delay_us(DMX_BREAK_US);

        // Drive PA9 HIGH → Mark After Break
        gpioa.bsrr().write(|w| w.bs9().set_bit());
        delay.delay_us(DMX_MAB_US);

        // Switch PA9 back to alternate function (MODER=10)
        gpioa.moder().modify(|r, w| {
            w.bits((r.bits() & !(0b11 << 18)) | (0b10 << 18))
        });

        // Re-enable USART transmitter
        usart1.cr1().modify(|_, w| w.te().set_bit());
    }
}

/// Blocking write of a single byte over UART.
fn block_write_byte(tx: &mut Tx<USART1>, byte: u8) {
    nb::block!(tx.write(byte)).ok();
}

/// Block until all bytes have been physically shifted out.
fn block_flush(tx: &mut Tx<USART1>) {
    let _ = tx.bflush();
}
