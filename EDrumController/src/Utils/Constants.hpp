#ifndef CONST_H
#define CONST_H

#include <stdint.h>

// MIDI defines
constexpr uint16_t MOMENTARY = 0U;
constexpr uint16_t LATCHING = 1U;
#define MODE MOMENTARY
// PINS
constexpr uint16_t NUM_KEYS = 16U;
#define CRASH_PIN A0
constexpr uint16_t CHOKE_PIN = 2U; // usable for interrupts on NANO
constexpr uint16_t HIHAT_PEDAL_PIN = 3U;
#define HIHAT_PIN A1

// SIGNALS
constexpr uint16_t NOTE_ON = 144U;
constexpr uint16_t NOTE_OFF = 128U;
constexpr uint16_t MIDI_MAX_VALUE = 127U;

// CRASH
constexpr uint8_t CHOKE_SIGNAL = 58U;
constexpr uint8_t CRASH_SIGNAL = 57U;
constexpr uint16_t CRASH_MIN_VAL = 320U;
constexpr uint16_t CRASH_MAX_VAL = 370U;
constexpr uint16_t CRASH_MID_VAL = 338U;
constexpr float CRASH_COEF = 0.4F;

// KICK
constexpr uint16_t KICK_IDLE_VALUE = 513U;
constexpr uint8_t KICK_MIDI_SIGNAL = 35U;
constexpr uint16_t KICK_PIN = 3U;

// HI-HAT
constexpr uint16_t HIHAT_PEDAL_SIGNAL = 65U;
constexpr uint16_t HIHAT_CLOSED_SIGNAL = 42U;
constexpr uint16_t HIHAT_OPEN_SIGNAL = 46U;
constexpr uint16_t HIHAT_MIN_VAL = 439U;
constexpr uint16_t HIHAT_MAX_VAL = 446U;
constexpr uint16_t HIHAT_DIVIDER = 250U;

constexpr uint8_t velocity = 100U; //velocity of MIDI notes, must be between 0 and 127
constexpr uint8_t noteON = 144U; //144 = 10010000 in binary, note on command
constexpr uint8_t noteOFF = 128U; //128 = 10000000 in binary, note off command

constexpr uint8_t debounceCycles = 100U;
constexpr uint16_t defaultIdleOffset = 4U;

#endif // CONST_H
