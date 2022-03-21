#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include <stdint.h>

// infinite response filter class
class IIRFilter
{
public:
    IIRFilter(float filterParameter, float initialValue = 0.0F)
        : _filterParameter(filterParameter)
        , _previousOutput(initialValue)
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
