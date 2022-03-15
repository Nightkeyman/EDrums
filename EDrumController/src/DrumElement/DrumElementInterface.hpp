#ifndef DRUM_ELEMENT_H
#define DRUM_ELEMENT_H

#include "../Utils/Constants.hpp"
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
        : _isDrumHit{false}
        , _midiSignal(midiSignal)
    { }

    // update state of drum element
    virtual void updateState(InputType inputSignal) = 0;

    // returns state of drum (true - hit, false - not hit)
    bool isDrumHit() const
    {
        return this->_isDrumHit;
    }

protected:
    // calls drum hit function with the argument value
    void hitDrum(const uint16_t hitVelocity)
    {
        this->sendMidiMessage(noteON, this->_midiSignal, hitVelocity);
    }

    bool _isDrumHit{false};

private:
    //send MIDI message through USB port
    void sendMidiMessage(const uint8_t command, const uint8_t MIDInote, const uint16_t MIDIvelocity)
    {
        Serial.write(command); //send note on or note off command
        Serial.write(MIDInote); //send pitch data
        Serial.write(MIDIvelocity); //send velocity data
    }

    const uint8_t _midiSignal;
};

#endif // DRUM_ELEMENT_H
