/// DMX512 protocol constants and application settings.

/// DMX512 baud rate (250 kbit/s).
pub const DMX_BAUD_RATE: u32 = 250_000;

/// DMX512 break duration in microseconds (min 88 µs, we use 176 µs for safety).
pub const DMX_BREAK_US: u32 = 176;

/// DMX512 Mark After Break (MAB) duration in microseconds (min 8 µs, we use 20 µs).
pub const DMX_MAB_US: u32 = 20;

/// Maximum number of DMX channels (slots). Full universe = 512.
/// We start with a smaller number to keep things simple.
pub const DMX_MAX_CHANNELS: usize = 64;

/// DMX start code (always 0x00 for normal dimmer data).
pub const DMX_START_CODE: u8 = 0x00;

// ---------------------------------------------------------------------------
// RGBW light – 4-channel mode
// ---------------------------------------------------------------------------
// DMX channels (1-indexed, relative to fixture start address)
pub const CH_RED: usize = 1;
pub const CH_GREEN: usize = 2;
pub const CH_BLUE: usize = 3;
pub const CH_WHITE: usize = 4;

// ---------------------------------------------------------------------------
// MIDI CC → RGBW mapping
// ---------------------------------------------------------------------------
// Which MIDI CC numbers control which colour. Adjust to taste.
pub const CC_RED: u8 = 1;
pub const CC_GREEN: u8 = 2;
pub const CC_BLUE: u8 = 3;
pub const CC_WHITE: u8 = 4;
