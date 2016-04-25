//
// Created by liam on 05/09/15.
//

#ifndef MADRAS_OSCILLATOR_H
#define MADRAS_OSCILLATOR_H

#include <cassert>
#include <cmath>

#include "StreamParameters.h"
#include "ParameterTypes/FloatSetterBase.h"

// Using theory from https://en.wikibooks.org/wiki/Sound_Synthesis_Theory/Oscillators_and_Wavetables#Wavetables

class Oscillator : public FloatSetterBase {
public:
    Oscillator(float frequency, int tableSize) {
        assert(tableSize != 0);
        this->tableSize = tableSize;
        this->setFreq(frequency);
        this->wavetable = new float[tableSize];
    }

    ~Oscillator() {
        delete this->wavetable;
    }

    void setFreq(float frequency) {
        this->frequency = frequency;
        this->stepSize = this->tableSize * (this->frequency /
                                            StreamParameters::SAMPLE_RATE);
    }

    float nextSample() {
        // Get next value using interpolation
        int posInt = (int) this->wavetablePos;
        float posDec = this->wavetablePos - posInt;
        float p1 = this->wavetable[posInt];

        // Make sure p2 wraps if necessary
        float p2;
        if ((posInt + 1) >= this->tableSize) {
            p2 = this->wavetable[0];
        } else {
            p2 = this->wavetable[posInt + 1];
        }

        float out = p1 + ((p2 - p1) * posDec);

        this->wavetablePos += this->stepSize;
        if (this->wavetablePos >= this->tableSize) {
            this->wavetablePos -= this->tableSize;
        }

        return out;
    };

protected:
    float wavetablePos = 0.0;
    float stepSize;
    int tableSize;
    float *wavetable = nullptr;
    float frequency;
};


#endif //MADRAS_OSCILLATOR_H
