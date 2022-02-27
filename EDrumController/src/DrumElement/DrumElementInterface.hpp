#ifndef DRUM_ELEMENT_H
#define DRUM_ELEMENT_H

#include "../Utils/Debouncer.hpp"
#include <stdint.h>

template <typename InputType>
class DrumElementBase
{
public:
    DrumElementBase(uint8_t midiSignal)
        : _isDrumHit{false}
        , _midiSignal(midiSignal)
    { }

    // update state of drum element
    virtual void updateState(InputType inputSignal) = 0;

    // returns state of drum (true - hit, false - not hit)
    virtual bool isDrumHit() const
    {
        return this->_isDrumHit;
    }

    // Returns midi signal value
    virtual uint8_t getMidiSignalValue() const
    {
        return this->_midiSignal;
    }

protected:
    const uint8_t _midiSignal;
    bool _isDrumHit{false};
};

#endif // DRUM_ELEMENT_H
