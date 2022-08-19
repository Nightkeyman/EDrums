#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include <stdint.h>

enum class DebounceState : uint8_t
{
    Debounce,
    Stable
};

// max debounce cycles - 255 (cycle counter is uint8)
template <typename ValueType>
class Debouncer
{
public:
    Debouncer(uint8_t debounceCycles)
        : _debounceCycles(debounceCycles)
        , _cycleCounter(debounceCycles)
    { }

    void updateState(ValueType currentValue)
    {
        if(currentValue == this->_lastValue)
        {
            this->_cycleCounter--;
            if(this->_cycleCounter == 0U)
            {
                this->_currentDebounceState = DebounceState::Stable;
            }
        }
        else
        {
            this->_cycleCounter = this->_debounceCycles;
            this->_currentDebounceState = DebounceState::Debounce;
        }
        this->_lastValue = currentValue;
    }

    void updateDebounceCycles(const uint8_t debounceCycles)
    {
        this->_debounceCycles = debounceCycles;
    }

    DebounceState getState() const
    {
        return this->_currentDebounceState;
    }

private:
    uint8_t _debounceCycles;
    ValueType _lastValue{0};
    uint8_t _cycleCounter;
    DebounceState _currentDebounceState{DebounceState::Debounce};
};

#endif // DEBOUNCER_H
