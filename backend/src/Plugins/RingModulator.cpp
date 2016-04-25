//
// Created by liam on 05/09/15.
//

#include "RingModulator.h"

RingModulator::RingModulator() : Plugin() {
    this->name = "RingModulator";
    this->description = "Multiplies the input signal by a waveform (for example a sine wave). Famously used in Dr Who for the Dalek voice (achieved using a sine wave at 30Hz).";

    // Create default values
    this->frequency = 30.0;
    this->setWaveshape("sine");

    // Add parameters to parameter vector
    this->addParameter(new WaveshapePluginParameter("waveshape", "The wave shape of the oscillator", "sine", this,
                                                    &RingModulator::setWaveshape));
    this->addParameter(new FloatSetterPluginParameter("frequency", "The frequency of the oscillator", &this->frequency,
                                                      static_cast<FloatSetterBase *>(this->oscillator.get()),
                                                      static_cast<void (FloatSetterBase::*)(
                                                              float)>(&Oscillator::setFreq), 0.0, 20000.0));
}

void RingModulator::setWaveshape(std::string shape) {
    if (shape == "sine") {
        this->oscillator = std::unique_ptr<SineOscillator>(new SineOscillator(this->frequency));
    }
}

void RingModulator::applyWet(float *out) {
    for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        out[i] = out[i] * this->oscillator->nextSample();
    }
}