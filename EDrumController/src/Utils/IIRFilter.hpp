#ifndef IIR_FILTER_H
#define IIR_FILTER_H

#include <stdint.h>

// infinite response filter class
class IIRFilter
{
public:
    IIRFilter(float filterParameter)
        : _filterParameter(filterParameter)
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
    float _previousOutput{0.0F};
    const float _filterParameter;
};

#endif // IIR_FILTER_H
