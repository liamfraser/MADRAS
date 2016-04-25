//
// Created by liam on 12/10/15.
//

#ifndef MADRAS_PITCHSHIFT_H
#define MADRAS_PITCHSHIFT_H

#include <cmath>
#include <cstring>
#include <fftw3.h>

#include "../../Plugin.h"

#define MAX_FRAME_LENGTH 1024

class PitchShift : public Plugin {
    // Lots of this based on
    // http://blogs.zynaptiq.com/bernsee/repo/smbPitchShift.cpp

    public:
        PitchShift();
        void applyWet(float *out);
        void doShift(float *out, float targetFreq);

    private:
        float shiftAmount;

        // Typical values are 1024, 2048 and 4096
        const int fftFrameSize = 1024;

        // STFT oversampling factor which also determines the overlap between adjacent STFT
        // frames. It should at least be 4 for moderate scaling ratios. A value of 32 is
        // recommended for best quality.
        const int oversampling = 4;

        // Lots of buffers and variables
        float inFIFO[MAX_FRAME_LENGTH];
        float outFIFO[MAX_FRAME_LENGTH];
        float FFTworksp[2*MAX_FRAME_LENGTH];
        float lastPhase[MAX_FRAME_LENGTH/2+1];
        float sumPhase[MAX_FRAME_LENGTH/2+1];
        float outputAccum[2*MAX_FRAME_LENGTH];
        float anaFreq[MAX_FRAME_LENGTH];
        float anaMagn[MAX_FRAME_LENGTH];
        float synFreq[MAX_FRAME_LENGTH];
        float synMagn[MAX_FRAME_LENGTH];
        long rover = false, init = false;
        long i,k, qpd, index, inFifoLatency, stepSize, fftFrameSize2;
        float freqPerBin, expct;

        static void fft(float *fftBuffer, int fftFrameSize, int sign);

        void setupPhase();
};


#endif //MADRAS_PITCHSHIFT_H
