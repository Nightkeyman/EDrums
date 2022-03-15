#include "ContinuousDrumElement.hpp"

void ContinuousDrumElement::updateState(const uint16_t inputSignal)
{
    // Filter, offset, abs
    uint16_t currentValue = this->getProcessedValue(inputSignal);
    // check if signal is above the threshold
    const bool isSignalAboveTreshold = this->isSignalAboveThreshold(currentValue);
    // check if semaphor flag is set
    if(this->_isHitBlocked == false)
    {
        // looking for the first peak after hit
        if(this->wasPeakReached(currentValue) & isSignalAboveTreshold)
        {
            this->hitDrum(this->getHitVelocity());
            // block next hit
            this->_isHitBlocked = true;
        }
    }
    else // if hit is blocked, debounce the state until its stable and idle
    {
        this->_debouncer.updateState(static_cast<int>(isSignalAboveTreshold));
        const bool isStateStable = (this->_debouncer.getState() == DebounceState::Stable);
        if(isStateStable & !isSignalAboveTreshold)
        {
            this->_isHitBlocked = false;
        }
    }
    // update previous value
    this->_previousValue = currentValue;
}

int ContinuousDrumElement::getProcessedValue(const uint16_t inputSignal)
{
    // Use simple IIR/PT1 filter
    const float filteredSignal = this->_filter.getUpdatedOutput(static_cast<float>(inputSignal));
    // centering to idle signal
    const float offsetSignal = filteredSignal - this->_idleValues.centre;
    // making the signal only positive
    const float aboluteSignal = abs(filteredSignal);
    // return fully processed signal
    return static_cast<int>(aboluteSignal);
}

bool ContinuousDrumElement::wasPeakReached(const uint16_t currentValue) const
{
    bool isPeakedReached = false;
    if(currentValue < this->_previousValue)
    {
        isPeakedReached = true;
    }
    return true;
}

bool ContinuousDrumElement::isSignalAboveThreshold(const uint16_t inputSignal) const
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
    // previous value was the peak of the signal
    return this->_previousValue;
}
