//
// Created by liam on 16/04/16.
//

#ifndef MADRAS_REVERB_H
#define MADRAS_REVERB_H

#include "../Plugin.h"
#include "../PluginComponents/CombFilter.h"
#include "../PluginComponents/AllPassFilter.h"
#include <cmath>

class Reverb : public Plugin, FloatSetterBase {
public:
    Reverb(float reverbTime = 500.0f) : Plugin() {
        this->name = "Reverb";
        this->description = "Schroeder Reverb";
        this->mix = 0.5f;

        this->reverbTime = reverbTime;

        this->addParameter(new FloatSetterPluginParameter("reverbTime",
                                                          "Reverb Time in milliseconds",
                                                          &this->reverbTime,
                                                          static_cast<FloatSetterBase *>(this),
                                                          static_cast<void (FloatSetterBase::*)(
                                                                  float)>(&Reverb::updateG),
                                                          0.0f,
                                                          5000.0f));

        // Make 4 comb filters using concert hall params
        for (unsigned int i = 0; i < NUMCOMBS; i++) {
            this->combs[i] = std::unique_ptr<CombFilter>(
                    new CombFilter(StreamParameters::msToSamples(this->concertHall[i][1])));
            this->combs[i]->delayLine->setDelay(StreamParameters::msToSamples(this->concertHall[i][1]));
            this->combs[i]->g = this->calcG(this->concertHall[i][1]);
        }

        // Make 2 all pass filters using concert hall params
        for (unsigned int i = 0; i < NUMAPS; i++) {
            int chIndex = i + NUMCOMBS;
            this->allPasses[i] = std::unique_ptr<AllPassFilter>(
                    new AllPassFilter(StreamParameters::msToSamples(this->concertHall[chIndex][1])));
            this->allPasses[i]->delayLine->setDelay(StreamParameters::msToSamples(this->concertHall[chIndex][1]));
            this->allPasses[i]->g = this->calcG(this->concertHall[chIndex][0]);
        }
    }

    void updateG(float reverbTime) {
        this->reverbTime = reverbTime;
        for (unsigned int i = 0; i < NUMCOMBS; i++) {
            this->combs[i]->g = this->calcG(this->concertHall[i][1]);
        }

        // Make 2 all pass filters using concert hall params
        for (unsigned int i = 0; i < NUMAPS; i++) {
            int chIndex = i + NUMCOMBS;
            this->allPasses[i]->g = this->calcG(this->concertHall[chIndex][0]);
        }
    }

    void applyWet(float *out) {
        for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
            float in = out[i];

            out[i] = 0.0f;
            // Do 4 comb filters in parallel
            for (auto &comb : this->combs) {
                out[i] += comb->next(in) / NUMCOMBS;
            }

            // Then the 2 all pass filters in series
            for (auto &ap : this->allPasses) {
                out[i] = ap->next(out[i]);
            }
        }
    }

private:
    float calcG(float delayTime) {
        return powf(0.001f, delayTime / this->reverbTime);
    }

    float reverbTime;

    static const int NUMCOMBS = 4;
    static const int NUMAPS = 2;
    std::unique_ptr<CombFilter> combs[NUMCOMBS];
    std::unique_ptr<AllPassFilter> allPasses[NUMAPS];

    // Suggested values for Schroeder reverb from https://christianfloisand.wordpress.com/2012/09/04/digital-reverberation/
    const float concertHall[6][2] = {
            {0.0f,   29.7f},
            {0.0f,   37.1f},
            {0.0f,   41.1f},
            {0.0f,   43.7f},
            {96.83f, 5.0f},
            {32.92f, 1.7f}
    };
};

#endif //MADRAS_REVERB_H
