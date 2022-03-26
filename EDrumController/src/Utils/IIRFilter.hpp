#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include <stdint.h>

/**
 * @brief Infinite response filter class
 *        Ts ~= 0.5ms, T_0.632 = (1/a) * Ts
 */
class IIRFilter
{
public:
    IIRFilter(float filterParameter, float initialValue = 0.0F)
        : _previousOutput(initialValue)
        , _filterParameter(filterParameter)
    { }

    float getUpdatedOutput(float updatedInput)
    {
        const float updatedOutput = ((1 - this->_filterParameter) * this->_previousOutput) +
                                    (this->_filterParameter * updatedInput);
        // update previous output
        this->_previousOutput = updatedOutput;
        return updatedOutput;
    }

private:
    float _previousOutput;
    const float _filterParameter;
};

#endif // IIR_FILTER_H
