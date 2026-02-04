pub struct IIRFilter {
    previous_output: f32,
    filter_parameter: f32,
}

impl IIRFilter {
    pub fn new(filter_parameter: f32, initial_value: f32) -> Self {
        Self {
            previous_output: initial_value,
            filter_parameter,
        }
    }

    pub fn get_updated_output(&mut self, updated_input: f32) -> f32 {
        let updated_output = ((1.0 - self.filter_parameter) * self.previous_output)
            + (self.filter_parameter * updated_input);
        self.previous_output = updated_output;
        updated_output
    }
}
