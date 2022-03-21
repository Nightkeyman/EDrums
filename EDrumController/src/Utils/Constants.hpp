#ifndef CONST_H
#define CONST_H

#include <stdint.h>

// MIDI defines
constexpr uint16_t MOMENTARY = 0;
constexpr uint16_t LATCHING = 1;
#define MODE MOMENTARY
// PINS
constexpr uint16_t NUM_KEYS = 16;
#define CRASH_PIN A0
constexpr uint16_t CHOKE_PIN = 2; // usable for interrupts on NANO
constexpr uint16_t HIHAT_PEDAL_PIN = 3;
#define HIHAT_PIN A1
constexpr uint16_t KICK_PIN = 3;
// SIGNALS
constexpr uint16_t KICK_SIGNAL = 35;
constexpr uint16_t CHOKE_SIGNAL = 58;
constexpr uint16_t CRASH_SIGNAL = 57;
constexpr uint16_t HIHAT_PEDAL_SIGNAL = 65;
constexpr uint16_t HIHAT_CLOSED_SIGNAL = 42;
constexpr uint16_t HIHAT_OPEN_SIGNAL = 46;
constexpr uint16_t NOTE_ON = 144;
constexpr uint16_t NOTE_OFF = 128;
// MIN & MAX VALUES
constexpr uint16_t MIDI_MAX_VALUE = 127;
constexpr uint16_t CRASH_MIN_VAL = 320;
constexpr uint16_t CRASH_MAX_VAL = 370;
constexpr uint16_t CRASH_MID_VAL = 338;
constexpr float CRASH_COEF = 0.4;
constexpr uint16_t HIHAT_MIN_VAL = 439;
constexpr uint16_t HIHAT_MAX_VAL = 446;
constexpr uint16_t HIHAT_DIVIDER = 250;

constexpr uint8_t velocity = 100; //velocity of MIDI notes, must be between 0 and 127
constexpr uint8_t noteON = 144; //144 = 10010000 in binary, note on command
constexpr uint8_t noteOFF = 128; //128 = 10000000 in binary, note off command

constexpr uint8_t debounce_cnt = 30U;

#endif // CONST_H
