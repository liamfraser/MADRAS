//
// Created by liam on 27/01/16.
//

#ifndef MADRAS_PITCHSHIFTWORKER_H
#define MADRAS_PITCHSHIFTWORKER_H

#include <thread>
#include "../../PluginComponents/Note.h"
#include "../../StreamParameters.h"
#include "PitchShift.h"

class PitchShiftWorker {
    public:
        PitchShiftWorker() {
            this->shifter = std::unique_ptr<PitchShift>(new PitchShift());
            this->note = nullptr;
            this->thread = nullptr;
        }

        void threadFunc() {
            this->shifter->doShift(samples, this->note->getFrequency());
        }

        void start() {
            if (!this->isFree()) {
                this->thread = std::unique_ptr<std::thread>(new std::thread(&PitchShiftWorker::threadFunc, this));
            }
        }

        void wait() {
            if (this->thread != nullptr) {
                this->thread->join();
            }
        }

        void setSamples(float *in) {
            for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
                this->samples[i] = in[i];
            }
        }

        unsigned int getNote() {
            if (this->note != nullptr) {
                return this->note->midiNum;
            }
            return 0;
        }

        void setNote(uint8_t midiNum, unsigned int validAt) {
            this->note = std::unique_ptr<Note>(new Note(midiNum, validAt));
        }

        bool isFree() {
            return this->note == nullptr;
        }

        void free() {
            this->note = nullptr;
            this->thread = nullptr;
        }

        float samples[StreamParameters::BUFFER_SIZE];

    private:
        std::unique_ptr<PitchShift> shifter;
        std::unique_ptr<std::thread> thread;
        std::unique_ptr<Note> note;
};

#endif //MADRAS_PITCHSHIFTWORKER_H
