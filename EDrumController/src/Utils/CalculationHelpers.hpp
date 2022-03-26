#ifndef CALCULATIONS_H
#define CALCULATIONS_H

// returns saturated value
template <typename ValueType>
static inline ValueType getLimitedValue(const ValueType inputValue, const ValueType topLimit)
{
    // previous value was the peak of the signal
    ValueType limitedValue = inputValue;
    if(limitedValue > topLimit)
    {
        limitedValue = topLimit;
    }
    return limitedValue;
}

#endif
