//
// Created by liam on 16/04/16.
//

#ifndef MADRAS_ALLPASSFILTER_H
#define MADRAS_ALLPASSFILTER_H

#include "DelayLineFilter.h"

class AllPassFilter : public DelayLineFilter {
public:
    AllPassFilter(unsigned int maxDelay) : DelayLineFilter(maxDelay) { };

    inline float next(float in) {
        // Subtle difference to the comb filter
        // It circulates a signal through a delay line
        float delayOut = this->delayLine->getOutput(in);

        // adding the delayed version, scaled with a constant, g, to the original.
        this->delayLine->writeInput(in + (delayOut * this->g));

        // Return the delayed sample + the input scaled by - g
        return delayOut - (this->g * in);
    }
};

#endif //MADRAS_ALLPASSFILTER_H
