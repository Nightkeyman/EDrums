#ifndef CONT_DRUM_ELEM_H
#define CONT_DRUM_ELEM_H

#include "../Utils/IIRFilter.hpp"
#include "DrumElementInterface.hpp"

class ContinuousDrumElement : public DrumElementBase<uint16_t>
{
public:
    ContinuousDrumElement(uint16_t idleSignal, uint8_t midiSignal)
        : _drumLimits{static_cast<uint16_t>(idleSignal - 20U),
                      static_cast<uint16_t>(idleSignal),
                      static_cast<uint16_t>(idleSignal + 20U)}
        , DrumElementBase(midiSignal)
    { }

    ContinuousDrumElement() = default;

    virtual void updateState(uint16_t inputSignal) override;

    // Returns value of the hit force
    uint8_t getHitVelocity() const;

private:
    uint8_t _hitVelocityValue{0U};

    IIRFilter _filter{0.1F};

    const struct hitValueLimits
    {
        uint16_t max;
        uint16_t centre;
        uint16_t min;
    } _drumLimits;
};

#endif // CONT_DRUM_ELEM_H
