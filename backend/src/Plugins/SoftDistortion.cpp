//
// Created by liam on 20/10/15.
//

#include "SoftDistortion.h"

SoftDistortion::SoftDistortion() : Plugin() {
    this->name = "SoftDistortion";
    this->description = "Distortion with configurable curve steepness";
    this->mix = 0.1f; // Don't want the default to be insane
}

inline float SoftDistortion::CubicAmplifier(float input) {
    // Non-linear amplifier with soft distortion curve
    float output, temp;
    if( input < 0.0 ) {
        temp = input + 1.0f;
        output = (temp * temp * temp) - 1.0f;
    } else {
        temp = input - 1.0f;
        output = (temp * temp * temp) + 1.0f;
    }
    return output;
}

void SoftDistortion::applyWet(float *out) {
    // Algorithm borrowed from http://portaudio.com/docs/v19-doxydocs/pa__fuzz_8c_source.html
    for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        out[i] = this->CubicAmplifier(this->CubicAmplifier(
                                      this->CubicAmplifier(
                                      this->CubicAmplifier(out[i]))));
        // Ensure sample isn't out of range we want it to clip so output gain actually works
        if (out[i] < -1.0f) {
            out[i] = -1.0f;
        } else if (out[i] > 1.0f) {
            out[i] = 1.0f;
        }
    }
}
