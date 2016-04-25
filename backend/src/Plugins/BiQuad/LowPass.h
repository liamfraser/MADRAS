//
// Created by liam on 06/10/15.
//

#ifndef MADRAS_LOWPASS_H
#define MADRAS_LOWPASS_H

#include "BiQuad.h"
#include "../../ParameterTypes/FloatSetterPluginParameter.h"
#include <cfloat>

class LowPass : public BiQuad {
public:
    LowPass() : BiQuad() {
        this->name = "LowPass";
        this->description = "A filter that only lets frequencies below the corner frequency through.";

        // Set default value
        this->setCornerFrequency(200);
        this->addParameter(new FloatSetterPluginParameter("cornerFrequency",
                                                          "The corner frequency of the low pass filter",
                                                          &this->cornerFrequency,
                                                          static_cast<FloatSetterBase *>(this),
                                                          static_cast<void (FloatSetterBase::*)(
                                                                  float)>(&LowPass::setCornerFrequency),
                                                          0.0,
                                                          20000.0));
    }

    void setCornerFrequency(float frequency) {
        // Designing Audio Effect Plug-Ins in C++ by Will Pirkle. Section 6.6.1 - 2nd order Butterworth
        this->cornerFrequency = frequency;
        float C = 1.0f / tanf(((float) M_PI * this->cornerFrequency) / StreamParameters::SAMPLE_RATE);
        this->a0 = (1.0f / (1.0f + sqrtf(2 * C) + powf(C, 2)));
        this->a1 = 2 * this->a0;
        this->a2 = this->a0;
        this->b1 = 2.0f * this->a0 * (1.0f - powf(C, 2));
        this->b2 = this->a0 * (1.0f - sqrtf(2 * C) + powf(C, 2));
    }

private:
    float cornerFrequency = 0.0;
};


#endif //MADRAS_LOWPASS_H
