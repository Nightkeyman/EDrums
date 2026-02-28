/// Simple MIDI message parser.
///
/// Handles running-status and filters for messages we care about
/// (Note On, Note Off, Control Change on channel 0).

use crate::constants::*;

/// A parsed MIDI message.
#[derive(Clone, Copy, Debug)]
pub enum MidiMessage {
    NoteOn { note: u8, velocity: u8 },
    NoteOff { note: u8, velocity: u8 },
    ControlChange { controller: u8, value: u8 },
}

/// Translates a USB-MIDI event packet (4 bytes) into an optional `MidiMessage`.
///
/// USB-MIDI packets are 32-bit:
///   byte 0: Cable Number (4 bit) | Code Index Number (4 bit)
///   byte 1: MIDI status
///   byte 2: data1
///   byte 3: data2
pub fn parse_usb_midi_packet(packet: &[u8; 4]) -> Option<MidiMessage> {
    let status = packet[1];
    let data1 = packet[2] & 0x7F;
    let data2 = packet[3] & 0x7F;

    // Mask channel nibble to check message type
    let msg_type = status & 0xF0;

    match msg_type {
        0x90 if data2 > 0 => Some(MidiMessage::NoteOn {
            note: data1,
            velocity: data2,
        }),
        0x90 => Some(MidiMessage::NoteOff {
            note: data1,
            velocity: 0,
        }),
        0x80 => Some(MidiMessage::NoteOff {
            note: data1,
            velocity: data2,
        }),
        0xB0 => Some(MidiMessage::ControlChange {
            controller: data1,
            value: data2,
        }),
        _ => None,
    }
}
