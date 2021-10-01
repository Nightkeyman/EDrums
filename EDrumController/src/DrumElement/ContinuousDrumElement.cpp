#include "ContinuousDrumElement.hpp"

void ContinuousDrumElement::updateState(uint16_t inputSignal)
{
    bool isDrumHit = false;
    // if (debouncer.getState == Stable)
    // {
    //     if(inputSignal < CRASH_MIN_VAL || inputSignal > CRASH_MAX_VAL)
    //     {
    //         hitCrash(crashHit);
    //     }
    // }

    // update internal state
    this->_isDrumHit = false;
}

uint8_t ContinuousDrumElement::getHitVelocity()
{
    return this->_hitVelocityValue;
}
