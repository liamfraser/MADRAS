//
// Created by liam on 16/11/15.
//

#ifndef MADRAS_CHORUS_H
#define MADRAS_CHORUS_H

#include "../PluginComponents/ModulatedDelayBase.h"

class Chorus : public ModulatedDelayBase {
public:
    Chorus() : ModulatedDelayBase(StreamParameters::msToSamples(7), StreamParameters::msToSamples(40)) {
        this->name = "Chorus";
        this->description = "Chorus";
        this->mix = 0.5f;

        this->depth = 50.0f;
        this->rate = 0.5f;
        this->feedback = 0.0f;
        this->setRate(0.2f);
        this->setFeedback(0.0f);
    }

private:
    inline float getDelaySamples() {
        float sineValue = (this->lfo->nextSample() + 1.0f) / 2.0f; // Shift oscillator so -1.0 and 1.0 are 0 and 1.0
        // Need to keep out of the min area with a chorus
        float delayBy = this->minDelaySamples +
                        (sineValue * (this->maxDelaySamples - this->minDelaySamples) * (this->depth / 100.0f));
        return delayBy;
    }
};


#endif //MADRAS_FLANGER_H
