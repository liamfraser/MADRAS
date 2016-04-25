//
// Created by liam on 06/10/15.
//

#ifndef MADRAS_HIGHPASS_H
#define MADRAS_HIGHPASS_H

#include "BiQuad.h"
#include "../../ParameterTypes/FloatSetterPluginParameter.h"
#include <cfloat>

class HighPass : public BiQuad {
public:
    HighPass() : BiQuad() {
        this->name = "HighPass";
        this->description = "A filter that only lets frequencies above the corner frequency through.";

        // Set default value
        this->setCornerFrequency(400);
        this->addParameter(
                new FloatSetterPluginParameter("cornerFrequency", "The corner frequency of the high pass filter",
                                               &this->cornerFrequency, static_cast<FloatSetterBase *>(this),
                                               static_cast<void (FloatSetterBase::*)(
                                                       float)>(&LowPass::setCornerFrequency), 0.0, 20000.0));
    }

    void setCornerFrequency(float frequency) {
        // Designing Audio Effect Plug-Ins in C++ by Will Pirkle. Section 6.6.1 - 2nd order Butterworth
        this->cornerFrequency = frequency;
        float C = tanf(((float) M_PI * this->cornerFrequency) / StreamParameters::SAMPLE_RATE);
        this->a0 = (1.0f / (1.0f + sqrtf(2 * C) + powf(C, 2)));
        this->a1 = -2 * this->a0;
        this->a2 = this->a0;
        this->b1 = 2.0f * this->a0 * (powf(C, 2) - 1.0f);
        this->b2 = this->a0 * (1.0f - sqrtf(2 * C) + powf(C, 2));
    }

private:
    float cornerFrequency = 0.0;
};


#endif //MADRAS_LOWPASS_H
