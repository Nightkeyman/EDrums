/// Maps incoming MIDI messages to DMX channel values.
///
/// RGBW 4-channel light fixture:
///   DMX ch 1 = Red   (0-255)
///   DMX ch 2 = Green (0-255)
///   DMX ch 3 = Blue  (0-255)
///   DMX ch 4 = White (0-255)
///
/// MIDI mapping:
///   CC 1 → Red,  CC 2 → Green,  CC 3 → Blue,  CC 4 → White
///   Note On  → all channels set to velocity-scaled brightness (white)
///   Note Off → all channels off

use crate::constants::*;
use crate::dmx::DmxTransmitter;
use crate::midi::MidiMessage;

/// Process one MIDI message and update the DMX universe accordingly.
pub fn apply_midi_to_dmx<DE>(msg: MidiMessage, dmx: &mut DmxTransmitter<DE>)
where
    DE: embedded_hal::digital::OutputPin,
{
    match msg {
        MidiMessage::ControlChange { controller, value } => {
            let brightness = scale_midi_to_dmx(value);
            match controller {
                CC_RED => dmx.set_channel(CH_RED, brightness),
                CC_GREEN => dmx.set_channel(CH_GREEN, brightness),
                CC_BLUE => dmx.set_channel(CH_BLUE, brightness),
                CC_WHITE => dmx.set_channel(CH_WHITE, brightness),
                _ => {} // ignore other CCs
            }
        }
        MidiMessage::NoteOn { note: _, velocity } => {
            // Quick demo: Note On sets all RGBW to velocity-scaled value
            let brightness = scale_midi_to_dmx(velocity);
            dmx.set_channel(CH_RED, brightness);
            dmx.set_channel(CH_GREEN, brightness);
            dmx.set_channel(CH_BLUE, brightness);
            dmx.set_channel(CH_WHITE, brightness);
        }
        MidiMessage::NoteOff { .. } => {
            dmx.set_channel(CH_RED, 0);
            dmx.set_channel(CH_GREEN, 0);
            dmx.set_channel(CH_BLUE, 0);
            dmx.set_channel(CH_WHITE, 0);
        }
    }
}

/// Scale a 7-bit MIDI value (0-127) to an 8-bit DMX value (0-255).
fn scale_midi_to_dmx(midi_val: u8) -> u8 {
    // 127 → 255, 0 → 0.  Simple: val * 2 + (val >> 6) ≈ val * 2.008
    let v = midi_val as u16;
    ((v * 255) / 127).min(255) as u8
}
