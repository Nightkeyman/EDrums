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
    const bool isStateStable = (this->_debouncer.getState() == DebounceState::Stable);

    if(isStateStable)
    {
        if(inputSignal == false)
        {
            // stable and choked - trigger if not blocked
            if(!this->_isHitBlocked)
            {
                this->_isHitBlocked = true;
                this->hitDrum(this->getHitVelocity());
            }
        }
        else
        {
            this->_isHitBlocked = false;
        }
    }

    this->_previousState = inputSignal;
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
