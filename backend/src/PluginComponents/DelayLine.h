//
// Created by liam on 16/11/15.
//

#ifndef MADRAS_DELAYLINE_H
#define MADRAS_DELAYLINE_H

#include <stdexcept>

class DelayLine {
public:
    DelayLine(int maxDelaySamples);
    ~DelayLine();
    void setDelay(float samples);
    void setFeedback(float feedback);
    float getOutput(float xn);
    void writeInput(float in);
    float doDelay(float xn);


private:
    int bufferSize;
    float *buf;
    int readIndex;
    int writeIndex;
    float feedback;
    float delaySamples; // Can be fractional
};


#endif //MADRAS_DELAYLINE_H
