//
// Created by liam on 16/04/16.
//

#ifndef MADRAS_DELAYLINEFILTER_H
#define MADRAS_DELAYLINEFILTER_H

#include <memory>
#include "DelayLine.h"
#include "../StreamParameters.h"

class DelayLineFilter {
public:
    DelayLineFilter(unsigned int maxDelay) {
        // Delay line by default has no feedback which is what we want
        this->delayLine = std::unique_ptr<DelayLine>(new DelayLine(maxDelay));
    };

    virtual float next(float in) = 0;

    // Gain constant of delayed signal
    float g;

    std::unique_ptr<DelayLine> delayLine;

};

#endif //MADRAS_DELAYLINEFILTER_H
