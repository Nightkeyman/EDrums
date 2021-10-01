#ifndef BINARY_DRUM_ELEM_H
#define BINARY_DRUM_ELEM_H

#include "DrumElementInterface.hpp"

class BinaryDrumElement : public DrumElementBase<bool>
{
public:
    virtual void updateState(bool inputSignal) override
    {
        // if (debouncer.getState == Stable)
        // {
        //     if(inputSignal < CRASH_MIN_VAL || inputSignal > CRASH_MAX_VAL)
        //     {
        //         hitCrash(crashHit);
        //     }
        // }
        this->_hitVelocityValue = inputSignal;
    }

    // Returns value of the hit force
    virtual uint8_t getHitVelocity()
    {
        return this->_hitVelocityValue;
    }

private:
    uint8_t _hitVelocityValue{0};

    const struct hitValueLimits
    {
        uint16_t max;
        uint16_t min;
    } _drumLimits;
};

#endif // BINARY_DRUM_ELEM_H