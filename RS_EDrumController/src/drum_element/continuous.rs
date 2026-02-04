use crate::constants::MIDI_MAX_VALUE;
use crate::drum_element::DrumElement;
use crate::utils::calculation_helpers::get_limited_value;
use crate::utils::debouncer::Debouncer;
use crate::utils::iir_filter::IIRFilter;
use crate::MidiOut;

pub struct ContinuousDrumElement {
    midi_signal: u8,
    filter: IIRFilter,
    previous_value: u16,
    idle_offset: u16,
    idle_centre: u16,
    debouncer: Debouncer<bool>,
    is_hit_blocked: bool,
}

impl ContinuousDrumElement {
    pub fn new(idle_signal: u16, midi_signal: u8, idle_offset: u16) -> Self {
        Self {
            midi_signal,
            filter: IIRFilter::new(0.2, idle_signal as f32),
            previous_value: idle_signal,
            idle_offset,
            idle_centre: idle_signal,
            debouncer: Debouncer::new(crate::constants::DEBOUNCE_CYCLES),
            is_hit_blocked: true,
        }
    }

    fn is_signal_above_threshold(&self, input: u16) -> bool {
        input > self.idle_offset
    }

    fn was_peak_reached(&self, current_value: u16) -> bool {
        current_value < self.previous_value
    }

    fn get_processed_value(&mut self, input: u16) -> i32 {
        let filtered = self.filter.get_updated_output(input as f32);
        let offset_signal = filtered - (self.idle_centre as f32);
        let abs_signal = if offset_signal < 0.0 {
            -offset_signal
        } else {
            offset_signal
        };
        abs_signal as i32
    }
}

impl DrumElement<u16> for ContinuousDrumElement {
    fn update_state(&mut self, input: u16, midi: &mut dyn MidiOut) {
        let current_value = self.get_processed_value(input) as u16;
        let is_signal_above = self.is_signal_above_threshold(current_value);
        self.debouncer.update_state(is_signal_above);

        if !self.is_hit_blocked {
            let was_peak = self.was_peak_reached(current_value);
            if was_peak && self.is_signal_above_threshold(self.previous_value) {
                midi.send(
                    crate::constants::NOTE_ON,
                    self.midi_signal,
                    self.get_hit_velocity(),
                );
                self.is_hit_blocked = true;
            }
        } else {
            let is_state_stable = matches!(
                self.debouncer.get_state(),
                crate::utils::debouncer::DebounceState::Stable
            );
            if is_state_stable && !is_signal_above {
                self.is_hit_blocked = false;
            }
        }

        self.previous_value = current_value;
    }
    fn get_hit_velocity(&self) -> u8 {
        let val = get_limited_value(self.previous_value as u8, MIDI_MAX_VALUE);
        val
    }
}
