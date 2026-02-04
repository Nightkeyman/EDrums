pub mod binary;
pub mod continuous;
pub mod continuous_binarizer;
use crate::MidiOut;

pub trait DrumElement<InputType> {
    fn update_state(&mut self, input: InputType, midi: &mut dyn MidiOut);
    fn get_hit_velocity(&self) -> u8;
}
