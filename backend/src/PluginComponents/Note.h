//
// Created by liam on 27/01/16.
//

#ifndef MADRAS_NOTE_H
#define MADRAS_NOTE_H

#include "MidiTable.h"
#include <cassert>
#include <vector>

class Note {
    public:
        Note(uint8_t midiNum, unsigned int validAt) {
            this->midiNum = midiNum;
            this->validAt = validAt;
        }

        float getFrequency() {
            return MIDITABLE[this->midiNum];
        }

        bool validNow(unsigned int index) {
            return index == validAt;
        }

        unsigned int midiNum;

    private:
        // Happened at sample n
        unsigned int validAt;
};

#endif //MADRAS_NOTE_H
