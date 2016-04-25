//
// Created by liam on 07/04/16.
//

#include "ModulatedDelayBase.h"
#include "../Oscillators/SineOscillator.h"
#include "DelayLine.h"
#include "../ParameterTypes/FloatSetterPluginParameter.h"


ModulatedDelayBase::ModulatedDelayBase(int minDelaySamples, int maxDelaySamples) : Plugin() {
    this->maxDelaySamples = maxDelaySamples;
    this->minDelaySamples = minDelaySamples;
    this->delayLine = std::unique_ptr<DelayLine>(new DelayLine(this->maxDelaySamples));
    this->lfo = std::unique_ptr<Oscillator>(static_cast<Oscillator *>(new SineOscillator(0.5f)));

    this->addParameter(new FloatSetterPluginParameter("feedback",
                                                      "Delay line feedback as %",
                                                      &this->feedback,
                                                      static_cast<FloatSetterBase *>(this),
                                                      static_cast<void (FloatSetterBase::*)(
                                                              float)>(&ModulatedDelayBase::setFeedback),
                                                      0.0f,
                                                      100.0f));

    this->addParameter(new FloatSetterPluginParameter("rate",
                                                      "Sweep rate in Hz",
                                                      &this->rate,
                                                      static_cast<FloatSetterBase *>(this),
                                                      static_cast<void (FloatSetterBase::*)(
                                                              float)>(&ModulatedDelayBase::setRate),
                                                      0.0f,
                                                      10.0f));

    this->addParameter(new FloatPluginParameter("depth",
                                                "Depth of sweep as %",
                                                &this->depth,
                                                0.0f,
                                                100.0f));
}

void ModulatedDelayBase::setFeedback(float feedback) {
    this->delayLine->setFeedback(feedback / 100.0f);
}

void ModulatedDelayBase::setRate(float rate) {
    this->lfo->setFreq(rate);
}

void ModulatedDelayBase::applyWet(float *out) {
    for (unsigned int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        // Update delay time from oscillator
        this->delayLine->setDelay(this->getDelaySamples());
        out[i] = this->delayLine->doDelay(out[i]);
    }
}