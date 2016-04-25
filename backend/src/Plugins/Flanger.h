//
// Created by liam on 16/11/15.
//

#ifndef MADRAS_FLANGER_H
#define MADRAS_FLANGER_H

#include "../PluginComponents/ModulatedDelayBase.h"

class Flanger : public ModulatedDelayBase {
public:
    Flanger() : ModulatedDelayBase(0, StreamParameters::msToSamples(10)) {
        this->name = "Flanger";
        this->description = "Flanger";
        this->mix = 0.5f;
        this->depth = 50.0f;
        this->rate = 0.5f;
        this->feedback = 30.0f;
        this->setRate(0.5f);
        this->setFeedback(30.0f);
    }

private:
    inline float getDelaySamples() {
        float sineValue = (this->lfo->nextSample() + 1.0f) / 2.0f; // Shift oscillator so -1.0 and 1.0 are 0 and 1.0
        float delayBy = sineValue * (this->maxDelaySamples) * (this->depth / 100.0f);
        return delayBy;
    }
};


#endif //MADRAS_FLANGER_H
