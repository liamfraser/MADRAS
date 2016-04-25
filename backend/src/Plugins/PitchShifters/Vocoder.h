//
// Created by liam on 22/11/15.
//

#ifndef MADRAS_VOCODER_H
#define MADRAS_VOCODER_H

// Unfortunately all this is too slow on a Pi2 - not tested on a Pi3

// So the big plan here is as follows - NOTE AT THE MOMENT DOING ANALYSIS MULTIPLE TIMES:

// On class initialisation, we will spawn three worker threads that will perform the pitch
// shifting and resynthesis part of the vocoding process. They will take pointers to any read only
// data and also pointers to three output buffers which we will mix.

// The initial analysis step will be done at the beginning of the applyWet callback

// The mixing step will be done at the end of the applyWet callback. We will need a way to message pass to the worker
// threads when they need to do work

// Initially going to try and be stupid about it and just spawn threads in the callback to see how that goes

#include "../../Plugin.h"
#include "PitchShiftWorker.h"

#include <cassert>
#include <jack/midiport.h>
#include <thread>

class Vocoder : public Plugin {
public:
    Vocoder();
    void applyWet(float *out);

private:
    void noteOn(uint8_t midiNote, unsigned int index);
    void noteOff(uint8_t midiNote, unsigned int index);
    void getMidiEvents();
    static const int MAXNOTES = 5;
    std::unique_ptr<PitchShiftWorker> noteWorkers[MAXNOTES];
};


#endif //MADRAS_VOCODER_H
