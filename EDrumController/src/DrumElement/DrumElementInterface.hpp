#ifndef DRUM_ELEMENT_H
#define DRUM_ELEMENT_H

#include "../Utils/Constants.hpp"
#include "../Utils/Debouncer.hpp"
#include <stdint.h>

#if GTEST_BUILD == 1
#    include "../Utils/FakeSerial.hpp"
#else
#    include "Arduino.h"
#endif

template <typename InputType>
class DrumElementBase
{
public:
    DrumElementBase(uint8_t midiSignal)
        : _isHitBlocked{true}
        , _midiSignal(midiSignal)
    { }

    // update state of drum element
    virtual void updateState(InputType inputSignal) = 0;

    // returns true if drum was recently hit
    virtual bool wasDrumHit() const
    {
        return (this->_isHitBlocked == true);
    }

    // Returns value of the hit force based on current value
    virtual uint8_t getHitVelocity() const = 0;

protected:
    // calls drum hit function with the argument value
    void hitDrum(const uint8_t hitVelocity)
    {
        this->sendMidiMessage(noteON, this->_midiSignal, hitVelocity);
    }

    // hit block flag - semaphor
    bool _isHitBlocked;

    // signal debouncer
    Debouncer<bool> _debouncer{debounceCycles};

private:
    //send MIDI message through USB port
    void sendMidiMessage(const uint8_t command, const uint8_t MIDInote, const uint8_t MIDIvelocity)
    {
        Serial.write(command); //send note on or note off command
        Serial.write(MIDInote); //send pitch data
        Serial.write(MIDIvelocity); //send velocity data
    }

    const uint8_t _midiSignal;
};

#endif // DRUM_ELEMENT_H
