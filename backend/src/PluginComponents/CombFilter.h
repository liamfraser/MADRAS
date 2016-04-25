//
// Created by liam on 16/04/16.
//

#ifndef MADRAS_COMBFILTER_H
#define MADRAS_COMBFILTER_H

#include "DelayLineFilter.h"

class CombFilter : public DelayLineFilter {
public:
    CombFilter(unsigned int maxDelay) : DelayLineFilter(maxDelay) { };

    inline float next(float in) {
        // It circulates a signal through a delay line
        float delayOut = this->delayLine->getOutput(in);

        // adding the delayed version, scaled with a constant, g, to the original.
        this->delayLine->writeInput(in + (delayOut * this->g));


        // Return the delayed sample
        return delayOut;
    }
};

#endif //MADRAS_COMBFILTER_H
