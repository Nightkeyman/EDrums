/*
  Drums.h - library for electronic drums elements.
  Created by Jan Sadlek, July 18, 2021.
*/
#ifndef DRUM_ELEMENT_H
#define DRUM_ELEMENT_H

#include <stdint.h>

template <typename InputType>
class DrumElementBase
{
public:
    DrumElementBase()
        : _isDrumHit{false}
        , _midiSignal()
    { }

    // update state of drum element
    virtual void updateState(InputType inputSignal) = 0;

    // returns state of drum (true - hit, false - not hit)
    virtual bool isDrumHit()
    {
        return this->_isDrumHit;
    }

    // Returns midi signal value
    virtual uint8_t getMidiSignalValue()
    {
        return this->_midiSignal;
    }

protected:
    const uint8_t _midiSignal;
    bool _isDrumHit{false};
};

#endif // DRUM_ELEMENT_H
