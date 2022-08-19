#ifndef BINARY_DRUM_ELEM_H
#define BINARY_DRUM_ELEM_H

#include "DrumElementInterface.hpp"

class BinaryDrumElement : public DrumElementBase<bool>
{
public:
    BinaryDrumElement(const uint8_t hitValue, uint8_t midiSignal);

    void updateState(const bool inputSignal) override;

    // Returns value of the hit force
    uint8_t getHitVelocity() const override;

private:
    bool isFallingEdge(bool currentState) const;

    uint8_t _hitValue;

    const uint8_t debounceCyclesAfterHit{10U};

    // input value from previous cycle
    bool _previousState;
};

#endif // BINARY_DRUM_ELEM_H
