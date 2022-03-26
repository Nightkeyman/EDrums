#include "../Utils/CalculationHelpers.hpp"
#include "BinaryElement.hpp"

BinaryDrumElement::BinaryDrumElement(const uint8_t hitValue, uint8_t midiSignal)
    : DrumElementBase(midiSignal)
    , _previousState(false)
{
    this->_hitValue = getLimitedValue<uint8_t>(hitValue, MIDI_MAX_VALUE);
}

void BinaryDrumElement::updateState(const bool inputSignal)
{
    this->_debouncer.updateState(inputSignal);
    bool isRisingEdge = this->isFallingEdge(inputSignal);
    if(this->_isHitBlocked == false)
    {
        if(isRisingEdge)
        {
            this->hitDrum(this->getHitVelocity());
            // block next hit
            this->_isHitBlocked = true;
        }
    }
    else // if hit is blocked, debounce the state until it's stable
    {
        const bool isStateStable = (this->_debouncer.getState() == DebounceState::Stable);
        if(isStateStable)
        {
            this->_isHitBlocked = false;
        }
    }
}

// Returns value of the hit force
uint8_t BinaryDrumElement::getHitVelocity() const
{
    return this->_hitValue;
}

bool BinaryDrumElement::isFallingEdge(bool currentState) const
{
    return ((this->_previousState == true) && (currentState == false));
}
