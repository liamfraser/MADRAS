//
// Created by liam on 05/09/15.
//

#ifndef MADRAS_STREAMPARAMETERS_H
#define MADRAS_STREAMPARAMETERS_H


class StreamParameters {
public:
    static const int SAMPLE_RATE = 44100;
    static const int BUFFER_SIZE = 64;

    static unsigned int msToSamples(float ms) {
        float samplesPerMS = SAMPLE_RATE / 1000.0f;
        return static_cast<unsigned int>(ms * samplesPerMS);
    }
};


#endif //MADRAS_STREAMPARAMETERS_H
