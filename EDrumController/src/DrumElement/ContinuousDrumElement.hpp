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
        : _idleValues{10U, static_cast<uint16_t>(idleSignal)}
        , DrumElementBase(midiSignal)
    { }

    virtual void updateState(const uint16_t inputSignal) override;

private:
    // Returns value of the hit force
    uint16_t getHitVelocity() const;

    bool isSignalAboveTreshold(const uint16_t inputSignal) const;

    bool isPeakReached(const uint16_t currentValue);

    int getProcessedValue(const uint16_t inputSignal);

    uint8_t _hitVelocityValue{0U};
    IIRFilter _filter{0.1F};
    Debouncer<int> _debouncer{20};
    bool _isHitBlocked{false};
    uint16_t _previousValue{0U};

    const struct hitValues
    {
        uint16_t offset;
        uint16_t centre;
    } _idleValues;
};

#endif // CONT_DRUM_ELEM_H
