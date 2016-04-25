//
// Created by liam on 05/09/15.
//

#ifndef MADRAS_SINEOSCILLATOR_H
#define MADRAS_SINEOSCILLATOR_H

#include "../Oscillator.h"

class SineOscillator : public Oscillator {
public:
    SineOscillator(float frequency, int tableSize = 360) : Oscillator(frequency, tableSize) {
        // Generate sine table
        for (int i = 0; i < this->tableSize; i++) {
            this->wavetable[i] = (float)sin((i * M_PI/(tableSize/2.0f)));
        }
    }
};


#endif //MADRAS_SINEOSCILLATOR_H
