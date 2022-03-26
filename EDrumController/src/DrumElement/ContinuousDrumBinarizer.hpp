#ifndef CONT_DRUM_BINARIZER_H
#define CONT_DRUM_BINARIZER_H

#include "ContinuousDrumElement.hpp"

class ContinuousDrumBinarizer : public ContinuousDrumElement
{
public:
    ContinuousDrumBinarizer(uint16_t idleSignal, uint8_t midiSignal)
        : ContinuousDrumElement(idleSignal, midiSignal)
    { }

    uint8_t getHitVelocity() const override;
};

#endif
