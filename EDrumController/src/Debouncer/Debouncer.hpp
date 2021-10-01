#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include "Arduino.h"

enum class DebounceState : uint8_t
{
    Debounce,
    Stable
};

class Debouncer
{
public:
    Debouncer(uint8_t cycles)
        : _debounceCycles(cycles)
    { }

    // update current debounce state
    void updateState();

    // returns current debounce state
    DebounceState getState();

private:
    uint8_t _debounceCycles;
};

#endif // DEBOUNCER_H