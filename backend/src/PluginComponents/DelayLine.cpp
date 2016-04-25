//
// Created by liam on 23/04/16.
//

#include "DelayLine.h"

// Circular delay line with support for feedback and fractional delays

DelayLine::DelayLine(int maxDelaySamples) {
    this->bufferSize = maxDelaySamples;
    this->buf = new float[this->bufferSize];

    // Initialise empty buffer
    for (int i = 0; i < this->bufferSize; i++) {
        this->buf[i] = 0.0f;
    }

    this->feedback = 0;
    this->readIndex = 0;
    this->writeIndex = 0;
    this->delaySamples = 0;
}

DelayLine::~DelayLine() {
    delete this->buf;
}

// Can be a fractional amount
void DelayLine::setDelay(float samples) {
    this->delaySamples = samples;
    readIndex = writeIndex - (unsigned int) delaySamples;
    if (readIndex < 0) {
        // Wrap
        readIndex += bufferSize;
    }
}

void DelayLine::setFeedback(float feedback) {
    this->feedback = feedback;
}

float DelayLine::getOutput(float xn) {
    // Get delay output
    float yn = this->buf[readIndex];

    // If delay is < 1 sample then interpolate between new input xn and x(n-1)
    if ((this->readIndex == this->writeIndex) && this->delaySamples < 1.0f) {
        yn = xn;
    }
    int prevIndex = this->readIndex - 1;
    // Wrap backwards if out of range
    if (prevIndex < 0) {
        prevIndex = this->bufferSize - 1;
    }
    float yn_1 = this->buf[prevIndex];

    // Work out the decimal part of the interpolation
    float decPart = this->delaySamples - (int) this->delaySamples;
    float interpolated = yn + ((yn_1 - yn) * decPart);

    if (this->delaySamples == 0) {
        // Pass input to output
        yn = xn;
    } else {
        yn = interpolated;
    }

    return yn;
}

void DelayLine::writeInput(float in) {
    this->buf[writeIndex] = in;

    // Increment pointers and wrap
    this->readIndex++;
    if (this->readIndex >= this->bufferSize) {
        this->readIndex = 0;
    }

    this->writeIndex++;
    if (this->writeIndex >= this->bufferSize) {
        this->writeIndex = 0;
    }
}

float DelayLine::doDelay(float xn) {
    // Get output
    float yn = this->getOutput(xn);

    // Write input to delay, taking feedback into account
    this->writeInput(xn + (yn * this->feedback));

    // Return output
    return yn;
}