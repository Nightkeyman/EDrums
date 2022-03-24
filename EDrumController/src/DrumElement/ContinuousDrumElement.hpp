#ifndef CONT_DRUM_ELEM_H
#define CONT_DRUM_ELEM_H

#include "../Utils/Debouncer.hpp"
#include "../Utils/IIRFilter.hpp"
#include "DrumElementInterface.hpp"
#include "math.h"
#include "stdlib.h"

class ContinuousDrumElement : public DrumElementBase<uint16_t>
{
public:
    ContinuousDrumElement(uint16_t idleSignal, uint8_t midiSignal)
        : _idleValues{4U, static_cast<uint16_t>(idleSignal)}
        , _previousValue{idleSignal}
        , DrumElementBase(midiSignal)
        , _filter(0.2F, idleSignal)
    { }

    virtual void updateState(const uint16_t inputSignal) override;

    // Returns value of the hit force based on current value
    uint8_t getHitVelocity() const;

private:
    // checks if signal is above the treshold
    bool isSignalAboveThreshold(const uint16_t inputSignal) const;

    // Returns true if value was bigger in previous cycle
    bool wasPeakReached(const uint16_t currentValue) const;

    // processes current value - filters, offsets, gets absolute value
    int getProcessedValue(const uint16_t inputSignal);

    // returns saturated value
    template <typename ValueType>
    ValueType getLimitedValue(const ValueType inputValue, const ValueType limit) const;

    // current output value
    uint8_t _hitVelocityValue{0U};

    // signal IIR filter
    IIRFilter _filter;

    // signal debouncer
    Debouncer<int> _debouncer{debounceCycles};

    // input value from previous cycle
    uint16_t _previousValue;

    const struct hitValues
    {
        uint16_t offset;
        uint16_t centre;
    } _idleValues;
};

#endif // CONT_DRUM_ELEM_H
