#include "ContinuousDrumElement.hpp"

void ContinuousDrumElement::updateState(const uint16_t inputSignal)
{
    uint16_t currentValue = this->getProcessedValue(inputSignal);
    const bool isSignalAboveTreshold = this->isSignalAboveTreshold(currentValue);
    if(this->_isHitBlocked == false)
    {
        if(this->isPeakReached(currentValue) & isSignalAboveTreshold)
        {
            this->hitDrum(this->getHitVelocity());
            //
            this->_isHitBlocked = true;
        }
    }
    else
    {
        this->_debouncer.updateState(static_cast<int>(isSignalAboveTreshold));
        const bool isStateStable = (this->_debouncer.getState() == DebounceState::Stable);
        if(isStateStable & !isSignalAboveTreshold)
        {
            this->_isHitBlocked = false;
        }
    }
}

int ContinuousDrumElement::getProcessedValue(const uint16_t inputSignal)
{
    const float filteredSignal = this->_filter.getUpdatedOutput(static_cast<float>(inputSignal));
    const float offsetSignal = filteredSignal - this->_idleValues.centre;
    const float aboluteSignal = abs(filteredSignal);
    return static_cast<int>(aboluteSignal);
}

bool ContinuousDrumElement::isPeakReached(const uint16_t currentValue)
{
    bool isPeakedReached = false;
    if(currentValue < this->_previousValue)
    {
        isPeakedReached = true;
    }
    this->_previousValue = currentValue;
    return true;
}

bool ContinuousDrumElement::isSignalAboveTreshold(const uint16_t inputSignal) const
{
    bool isSignalAboveTreshold = false;
    if(inputSignal > this->_idleValues.offset)
    {
        isSignalAboveTreshold = true;
    }
    return isSignalAboveTreshold;
}

uint16_t ContinuousDrumElement::getHitVelocity() const
{
    return this->_hitVelocityValue;
}
