use crate::MidiOut;

pub struct DummyMidi;

impl MidiOut for DummyMidi {
    fn send(&mut self, _command: u8, _note: u8, _velocity: u8) {
        // stub: wiring to real UART/USB should be implemented per target
    }
}

// Simple hardware-adapter stubs used by the example main.
pub fn analog_read(_pin: u8) -> u16 {
    0
}

pub fn digital_read(_pin: u8) -> bool {
    false
}
