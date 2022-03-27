#ifndef CONT_DRUM_ELEM_H
#define CONT_DRUM_ELEM_H

#include "../Utils/IIRFilter.hpp"
#include "DrumElementInterface.hpp"
#include "math.h"
#include "stdlib.h"

class ContinuousDrumElement : public DrumElementBase<uint16_t>
{
public:
    ContinuousDrumElement(uint16_t idleSignal,
                          uint8_t midiSignal,
                          uint16_t idleOffset = defaultIdleOffset)
        : DrumElementBase(midiSignal)
        , _filter(0.2F, idleSignal)
        , _previousValue{idleSignal}
        , _idleValues{idleOffset, static_cast<uint16_t>(idleSignal)}
    { }

    void updateState(const uint16_t inputSignal) override;

    uint8_t getHitVelocity() const override;

private:
    // checks if signal is above the treshold
    bool isSignalAboveThreshold(const uint16_t inputSignal) const;

    // Returns true if value was bigger in previous cycle
    bool wasPeakReached(const uint16_t currentValue) const;

    // processes current value - filters, offsets, gets absolute value
    int getProcessedValue(const uint16_t inputSignal);

    // signal IIR filter
    IIRFilter _filter;

    // input value from previous cycle
    uint16_t _previousValue;

    const struct hitValues
    {
        uint16_t offset;
        uint16_t centre;
    } _idleValues;
};

#endif // CONT_DRUM_ELEM_H
