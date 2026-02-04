use crate::drum_element::continuous::ContinuousDrumElement;
use crate::drum_element::DrumElement;
use crate::MidiOut;

pub struct ContinuousDrumBinarizer {
    inner: ContinuousDrumElement,
}

impl ContinuousDrumBinarizer {
    pub fn new(idle_signal: u16, midi_signal: u8, idle_offset: u16) -> Self {
        Self {
            inner: ContinuousDrumElement::new(idle_signal, midi_signal, idle_offset),
        }
    }
}

impl DrumElement<u16> for ContinuousDrumBinarizer {
    fn update_state(&mut self, input: u16, midi: &mut dyn MidiOut) {
        self.inner.update_state(input, midi);
    }
    fn get_hit_velocity(&self) -> u8 {
        crate::constants::MIDI_MAX_VALUE
    }
}
