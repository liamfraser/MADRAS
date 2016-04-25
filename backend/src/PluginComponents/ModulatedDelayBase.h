//
// Created by liam on 07/04/16.
//

#ifndef MADRAS_MODULATEDDELAYBASE_H
#define MADRAS_MODULATEDDELAYBASE_H

// Base for Chorus and Flanger

#include "../Plugin.h"
#include "../ParameterTypes/FloatSetterBase.h"
#include "DelayLine.h"
#include "../Oscillator.h"

class ModulatedDelayBase : public Plugin, public FloatSetterBase {
public:
    ModulatedDelayBase(int minDelaySamples, int maxDelaySamples);
    void applyWet(float *out);
    void setRate(float rate);
    void setFeedback(float feedback);

private:
    std::unique_ptr<DelayLine> delayLine;
    virtual float getDelaySamples() = 0;

protected:
    std::unique_ptr<Oscillator> lfo;
    int maxDelaySamples;
    int minDelaySamples;
    float rate; // In Hz
    float depth; // Depth is a percentage of how much of the delay line will be used
    float feedback; // For delay line
};


#endif //MADRAS_MODULATEDDELAYBASE_H
