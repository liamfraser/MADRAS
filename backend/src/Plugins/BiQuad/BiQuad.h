//
// Created by liam on 06/10/15.
//

#ifndef MADRAS_BIQUAD_H
#define MADRAS_BIQUAD_H


#include "../../Plugin.h"
#include "../../StreamParameters.h"
#include "../../ParameterTypes/FloatSetterBase.h"

#include <cmath>

class BiQuad : public Plugin, public FloatSetterBase {
    // To help with understanding look at diagram here:
    // https://en.wikipedia.org/wiki/Digital_biquad_filter
    // Also see Designing Audio Effect Plug-Ins in C++ by Will Pirkle. Section 6.6 - Generic Bi-Quad designs

public:
    BiQuad() : Plugin() {
        // Zero all delay elements
        this->xn_1 = 0.0;
        this->xn_2 = 0.0;
        this->yn_1 = 0.0;
        this->yn_2 = 0.0;
    }

    void applyWet(float *out) {
        // y(n) = a0x(n) + a1x(n - 1) + a2x(n - 2) - b1y(n - 1) - b2y(n - 2)
        for (int n = 0; n < StreamParameters::BUFFER_SIZE; n++) {
            float xn = out[n];
            float yn = (this->a0 * xn)
                       + (this->a1 * this->xn_1)
                       + (this->a2 * this->xn_2)
                       - (this->b1 * this->yn_1)
                       - (this->b2 * this->yn_2);

            // Update delay elements
            this->xn_2 = this->xn_1;
            this->xn_1 = xn;
            this->yn_2 = this->yn_1;
            this->yn_1 = yn;

            // Finally update output buffer
            out[n] = yn;
        }
    }

    virtual void setCornerFrequency(float frequency) = 0;

protected:
    // Filter coefficients
    float a0, a1, a2, b1, b2 = 0.0;
    // Delay elements x(n-1), x(n-2) etc...
    float xn_1, xn_2, yn_1, yn_2 = 0.0;
};


#endif //MADRAS_BIQUAD_H
