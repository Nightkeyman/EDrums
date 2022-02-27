// MIDI defines
#define MOMENTARY 0
#define LATCHING 1
#define MODE MOMENTARY
// PINS
#define NUM_KEYS 16
#define CRASH_PIN A0
#define CHOKE_PIN 2 // usable for interrupts on NANO
#define HIHAT_PEDAL_PIN 3
#define HIHAT_PIN A1
#define KICK_PIN 3
// SIGNALS
#define KICK_SIGNAL 35
#define CHOKE_SIGNAL 58
#define CRASH_SIGNAL 57
#define HIHAT_PEDAL_SIGNAL 65
#define HIHAT_CLOSED_SIGNAL 42
#define HIHAT_OPEN_SIGNAL 46
#define NOTE_ON 144
#define NOTE_OFF 128
// MIN & MAX VALUES
#define MIDI_MAX_VALUE 127
#define CRASH_MIN_VAL 320
#define CRASH_MAX_VAL 370
#define CRASH_MID_VAL 338
#define CRASH_COEF 0.4
#define HIHAT_MIN_VAL 439
#define HIHAT_MAX_VAL 446
#define HIHAT_DIVIDER 250

constexpr uint8_t velocity = 100; //velocity of MIDI notes, must be between 0 and 127
constexpr uint8_t noteON = 144; //144 = 10010000 in binary, note on command
constexpr uint8_t noteOFF = 128; //128 = 10000000 in binary, note off command

constexpr uint8_t debounce_cnt = 30U;
