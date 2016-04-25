//
// Created by liam on 12/10/15.
//

#include "PitchShift.h"

// Lots of this based on
// http://blogs.zynaptiq.com/bernsee/repo/smbPitchShift.cpp

PitchShift::PitchShift() : Plugin() {
    this->name = "PitchShift";
    this->description = "Shifts the pitch of the input. 1.0 is normal pitch.";

    // Set default values
    // 0.5 is one octave down and 2.0 is one octave up
    // See https://en.wikipedia.org/wiki/Equal_temperament#Calculating_absolute_frequencies
    this->shiftAmount = 1.0f/powf(2, 5.0f/12.0f);

    // Add parameters
    this->addParameter(new FloatPluginParameter("shiftAmount", "Amount to shift by", &this->shiftAmount, 0.5, 2));

    this->setupPhase();
}

void PitchShift::applyWet(float *out) {
    if (this->shiftAmount == 1.0) {
        // Why do work if we don't have to :)
        return;
    }

    this->doShift(out, -1.0f);
}

void PitchShift::fft(float *fftBuffer, int fftFrameSize, int sign) {
    // http://www.fftw.org/fftw3_doc/Complex-One_002dDimensional-DFTs.html#Complex-One_002dDimensional-DFTs

    fftwf_complex *buf = (fftwf_complex*)fftBuffer;

    if (sign == -1) {
        sign = FFTW_FORWARD;
    } else {
        sign = FFTW_BACKWARD;
    }

    auto p = fftwf_plan_dft_1d(fftFrameSize, buf, buf, sign, FFTW_ESTIMATE);

    fftwf_execute(p);
    fftwf_destroy_plan(p);
}

void PitchShift::setupPhase() {
    /* set up some handy variables */
    fftFrameSize2 = fftFrameSize/2;
    stepSize = fftFrameSize/oversampling;
    freqPerBin = StreamParameters::SAMPLE_RATE/(float)fftFrameSize;
    expct = 2.*M_PI*(float)stepSize/(float)fftFrameSize;
    inFifoLatency = fftFrameSize-stepSize;
    if (rover == false) rover = inFifoLatency;

    /* initialize our static arrays */
    if (init == false) {
        memset(inFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
        memset(outFIFO, 0, MAX_FRAME_LENGTH * sizeof(float));
        memset(FFTworksp, 0, 2 * MAX_FRAME_LENGTH * sizeof(float));
        memset(lastPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(float));
        memset(sumPhase, 0, (MAX_FRAME_LENGTH / 2 + 1) * sizeof(float));
        memset(outputAccum, 0, 2 * MAX_FRAME_LENGTH * sizeof(float));
        memset(anaFreq, 0, MAX_FRAME_LENGTH * sizeof(float));
        memset(anaMagn, 0, MAX_FRAME_LENGTH * sizeof(float));
        init = true;
    }
}

void PitchShift::doShift(float *out, float targetFreq = -1.0) {
    float magn, phase, tmp, window, real, imag;

    for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        /* As long as we have not yet collected enough data just read in */
        inFIFO[rover] = out[i];
        out[i] = outFIFO[rover - inFifoLatency];
        rover++;

        /* now we have enough data for processing */
        if (rover >= fftFrameSize) {
            rover = inFifoLatency;

            /* do windowing and re,im interleave */
            for (k = 0; k < fftFrameSize; k++) {
                // Hamming window
                window = -.5 * cos(2. * M_PI * (float) k / (float) fftFrameSize) + .5;
                FFTworksp[2 * k] = inFIFO[k] * window;
                FFTworksp[2 * k + 1] = 0.;
            }


            /* ***************** ANALYSIS ******************* */
            /* do transform */
            fft(FFTworksp, fftFrameSize, -1);

            /* this is the analysis step */
            for (k = 0; k <= fftFrameSize2; k++) {

                /* de-interlace FFT buffer */
                real = FFTworksp[2 * k];
                imag = FFTworksp[2 * k + 1];

                /* compute magnitude and phase */
                magn = 2. * sqrt(real * real + imag * imag);
                phase = atan2(imag, real);

                /* compute phase difference */
                tmp = phase - lastPhase[k];
                lastPhase[k] = phase;

                /* subtract expected phase difference */
                tmp -= (float) k * expct;

                /* map delta phase into +/- Pi interval */
                qpd = tmp / M_PI;
                if (qpd >= 0) qpd += qpd & 1;
                else qpd -= qpd & 1;
                tmp -= M_PI * (float) qpd;

                /* get deviation from bin frequency from the +/- Pi interval */
                tmp = oversampling * tmp / (2. * M_PI);

                /* compute the k-th partials' true frequency */
                tmp = (float) k * freqPerBin + tmp * freqPerBin;

                /* store magnitude and true frequency in analysis arrays */
                anaMagn[k] = magn;
                anaFreq[k] = tmp;
            }

            // Work out frequency of note and then the shift amount
            if (targetFreq != -1.0) {
                int max_i = 0;
                for (k = 0; k <= fftFrameSize2; k++) {
                    if (anaMagn[k] > anaMagn[max_i]) {
                        max_i = k;
                    }
                }
                float noteFreq = anaFreq[max_i];

                this->shiftAmount = targetFreq / noteFreq;
                if (this->shiftAmount < 0.0 || this->shiftAmount > 5.0) {
                    this->shiftAmount = 1.0;
                }
            }

            /* ***************** PROCESSING ******************* */
            /* this does the actual pitch shifting */
            memset(synMagn, 0, fftFrameSize*sizeof(float));
            memset(synFreq, 0, fftFrameSize*sizeof(float));
            for (k = 0; k <= fftFrameSize2; k++) {
                index = k * shiftAmount;
                if (index <= fftFrameSize2) {
                    synMagn[index] += anaMagn[k];
                    synFreq[index] = anaFreq[k] * shiftAmount;
                }
            }

            /* ***************** SYNTHESIS ******************* */
            /* this is the synthesis step */
            for (k = 0; k <= fftFrameSize2; k++) {

                /* get magnitude and true frequency from synthesis arrays */
                magn = synMagn[k];
                tmp = synFreq[k];

                /* subtract bin mid frequency */
                tmp -= (float)k*freqPerBin;

                /* get bin deviation from freq deviation */
                tmp /= freqPerBin;

                /* take oversamp into account */
                tmp = 2.*M_PI*tmp/oversampling;

                /* add the overlap phase advance back in */
                tmp += (float)k*expct;

                /* accumulate delta phase to get bin phase */
                sumPhase[k] += tmp;
                phase = sumPhase[k];

                /* get real and imag part and re-interleave */
                FFTworksp[2*k] = magn*cos(phase);
                FFTworksp[2*k+1] = magn*sin(phase);
            }

            /* zero negative frequencies */
            for (k = fftFrameSize+2; k < 2*fftFrameSize; k++) {
                FFTworksp[k] = 0.;
            }

            /* do inverse transform */
            fft(FFTworksp, fftFrameSize, 1);

            /* do windowing and add to output accumulator */
            for(k=0; k < fftFrameSize; k++) {
                window = -.5*cos(2.*M_PI*(float)k/(float)fftFrameSize)+.5;
                outputAccum[k] += 2.*window*FFTworksp[2*k]/(fftFrameSize2*oversampling);
            }

            for (k = 0; k < stepSize; k++) {
                outFIFO[k] = outputAccum[k];
            }

            /* shift accumulator */
            memmove(outputAccum, outputAccum+stepSize, fftFrameSize*sizeof(float));

            /* move input FIFO */
            for (k = 0; k < inFifoLatency; k++) {
                inFIFO[k] = inFIFO[k+stepSize];
            }
        }

    }
}