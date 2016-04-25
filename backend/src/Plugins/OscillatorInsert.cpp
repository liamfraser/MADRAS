//
// Created by liam on 05/09/15.
//

#include "OscillatorInsert.h"

OscillatorInsert::OscillatorInsert() : Plugin() {
    this->name = "OscillatorInsert";
    this->description = "Used for testing oscillators.";
    this->oscillator = std::unique_ptr<Oscillator>(static_cast<Oscillator*>(new SineOscillator(440)));
}

void OscillatorInsert::applyWet(float *out) {
    for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        out[i] = this->oscillator->nextSample();
    }
}
