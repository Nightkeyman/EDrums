use crate::constants::DEBOUNCE_CYCLES;
use crate::drum_element::DrumElement;
use crate::utils::calculation_helpers::get_limited_value;
use crate::utils::debouncer::Debouncer;
use crate::MidiOut;

pub struct BinaryDrumElement {
    midi_signal: u8,
    hit_value: u8,
    previous_state: bool,
    is_hit_blocked: bool,
    debouncer: Debouncer<bool>,
}

impl BinaryDrumElement {
    pub fn new(hit_value: u8, midi_signal: u8) -> Self {
        Self {
            midi_signal,
            hit_value: get_limited_value(hit_value, crate::constants::MIDI_MAX_VALUE),
            previous_state: false,
            is_hit_blocked: true,
            debouncer: Debouncer::new(DEBOUNCE_CYCLES),
        }
    }
}

impl DrumElement<bool> for BinaryDrumElement {
    fn update_state(&mut self, input: bool, midi: &mut dyn MidiOut) {
        self.debouncer.update_state(input);
        let is_state_stable = matches!(
            self.debouncer.get_state(),
            crate::utils::debouncer::DebounceState::Stable
        );

        if is_state_stable {
            if input == false {
                if !self.is_hit_blocked {
                    self.is_hit_blocked = true;
                    midi.send(
                        crate::constants::NOTE_ON,
                        self.midi_signal,
                        self.get_hit_velocity(),
                    );
                }
            } else {
                self.is_hit_blocked = false;
            }
        }

        self.previous_state = input;
    }
    fn get_hit_velocity(&self) -> u8 {
        self.hit_value
    }
}
