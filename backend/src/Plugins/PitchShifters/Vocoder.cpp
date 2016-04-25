//
// Created by liam on 22/11/15.
//

#include "Vocoder.h"

Vocoder::Vocoder() {
    this->name = "Vocoder";
    this->description = "Vocoder";
    this->mix = 0.8f;

    for (int i = 0; i < this->MAXNOTES; i++) {
        this->noteWorkers[i] = std::unique_ptr<PitchShiftWorker>(new PitchShiftWorker());
    }
}

void Vocoder::getMidiEvents() {
    // https://github.com/jackaudio/jack2/blob/master/example-clients/midi_dump.c
    assert(this->midiBuffer != nullptr);

    jack_midi_event_t inputEvent;
    jack_nframes_t evCount = jack_midi_get_event_count(this->midiBuffer);
    for (uint32_t i = 0; i < evCount; i++) {
        jack_midi_event_get(&inputEvent, this->midiBuffer, i);

        uint8_t type = inputEvent.buffer[0];
        if (type == 0x90) {
            // Note On
            assert(inputEvent.size == 3);
            noteOn(inputEvent.buffer[1], inputEvent.time);
        } else if (type == 0x80) {
            // Note off
            assert(inputEvent.size == 3);
            noteOff(inputEvent.buffer[1], inputEvent.time);
        }
    }
}

void Vocoder::noteOn(uint8_t midiNote, unsigned int index) {
    for (int i = 0; i < this->MAXNOTES; i++) {
        if (this->noteWorkers[i]->getNote() == midiNote) {
            printf("Note %d already on\n", midiNote);
            return;
        }
    }

    for (int i = 0; i < this->MAXNOTES; i++) {
        if (this->noteWorkers[i]->isFree()) {
            this->noteWorkers[i]->setNote(midiNote, index);
            printf("Note %d (%f) turned on\n", midiNote, MIDITABLE[midiNote]);
            return;
        }
    }

    // If we get here no free workers
    printf("MAXNOTES. Can't play note %d\n", midiNote);
}

void Vocoder::noteOff(uint8_t midiNote, __attribute__ ((unused))unsigned int index) {
    for (int j = 0; j < this->MAXNOTES; j++) {
        if (this->noteWorkers[j]->getNote() == midiNote) {
            this->noteWorkers[j]->free();
            printf("Note %d (%f) turned off\n", midiNote, MIDITABLE[midiNote]);
            return;
        }
    }
}

void Vocoder::applyWet(float *out) {
    // Can't do anything
    if (this->midiBuffer == nullptr) {
        return;
    }
    getMidiEvents();

    for(int i = 0; i < this->MAXNOTES; i++) {
        this->noteWorkers[i]->setSamples(out);
        this->noteWorkers[i]->start();
    }

    // Join
    for(int i = 0; i < this->MAXNOTES; i++) {
        this->noteWorkers[i]->wait();
    }

    // Mix output of workers as output signal
    for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        float shifted = 0.0f;
        for (int j = 0; j < this->MAXNOTES; j++) {
            if (!this->noteWorkers[j]->isFree()) {
                shifted += this->noteWorkers[j]->samples[i] * (1.0f/this->MAXNOTES);
            }
        }
        out[i] = shifted;
    }
}