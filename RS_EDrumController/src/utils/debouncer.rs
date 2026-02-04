use core::marker::Copy;

#[derive(Copy, Clone, PartialEq, Eq)]
pub enum DebounceState {
    Debounce,
    Stable,
}

pub struct Debouncer<ValueType>
where
    ValueType: Copy + PartialEq,
{
    debounce_cycles: u8,
    last_value: ValueType,
    cycle_counter: u8,
    current_state: DebounceState,
}

impl<ValueType> Debouncer<ValueType>
where
    ValueType: Copy + PartialEq + Default,
{
    pub fn new(debounce_cycles: u8) -> Self {
        Self {
            debounce_cycles,
            last_value: ValueType::default(),
            cycle_counter: debounce_cycles,
            current_state: DebounceState::Debounce,
        }
    }

    pub fn update_state(&mut self, current_value: ValueType) {
        if current_value == self.last_value {
            if self.cycle_counter > 0 {
                self.cycle_counter = self.cycle_counter.wrapping_sub(1);
            }
            if self.cycle_counter == 0 {
                self.current_state = DebounceState::Stable;
            }
        } else {
            self.cycle_counter = self.debounce_cycles;
            self.current_state = DebounceState::Debounce;
        }
        self.last_value = current_value;
    }

    pub fn get_state(&self) -> DebounceState {
        self.current_state
    }
}
