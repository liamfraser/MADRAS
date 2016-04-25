//
// Created by liam on 05/09/15.
//

#ifndef MADRAS_OSCILLATORINSERT_H
#define MADRAS_OSCILLATORINSERT_H

#include <memory>

#include "../Plugin.h"
#include "../StreamParameters.h"
#include "../Oscillator.h"
#include "../Oscillators/SineOscillator.h"

class OscillatorInsert : public Plugin {
    public:
        OscillatorInsert();
        void applyWet(float *out);

    private:
        std::unique_ptr<Oscillator> oscillator;
};


#endif //MADRAS_OSCILLATORINSERT_H
