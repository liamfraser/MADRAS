//
// Created by liam on 23/10/15.
//

#include "Delay.h"
#include "../ParameterTypes/FloatSetterPluginParameter.h"

Delay::Delay() : Plugin() {
    this->name = "Delay";
    this->description = "Feedback delay";
    this->delayLine = std::unique_ptr<DelayLine>(new DelayLine(StreamParameters::msToSamples(this->maxDelayMS)));
    this->mix = 0.5f;
    this->feedback = 0.5f;
    this->setDelay(500); // Start delay with 0.5s of delay

    // Set up parameters
    this->addParameter(new FloatSetterPluginParameter("delayTime",
                                                      "Time in milliseconds to delay for",
                                                      &this->delayForMS,
                                                      static_cast<FloatSetterBase *>(this),
                                                      static_cast<void (FloatSetterBase::*)(float)>(&Delay::setDelay),
                                                      0.0f,
                                                      this->maxDelayMS));

    this->addParameter(new FloatSetterPluginParameter("feedback",
                                                      "Delay line feedback as %",
                                                      &this->feedback,
                                                      static_cast<FloatSetterBase *>(this),
                                                      static_cast<void (FloatSetterBase::*)(
                                                              float)>(&Delay::setFeedback),
                                                      0.0f,
                                                      100.0f));
    this->feedback = 50.0f;
    this->setFeedback(this->feedback);
}

void Delay::setFeedback(float feedback) {
    this->delayLine->setFeedback(feedback / 100.0f);
}

void Delay::setDelay(float delayMS) {
    if (delayMS > this->maxDelayMS) {
        throw std::range_error("Delay higher than max range");
    }
    this->delayForMS = delayMS;
    this->delayFor = StreamParameters::msToSamples(delayMS);
    this->delayLine->setDelay(delayFor);
}

void Delay::applyWet(float *out) {
    for (unsigned int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        out[i] = this->delayLine->doDelay(out[i]);
    }
}