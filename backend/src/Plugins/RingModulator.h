//
// Created by liam on 05/09/15.
//

#ifndef MADRAS_RINGMODULATOR_H
#define MADRAS_RINGMODULATOR_H

#include <memory>

#include "../Plugin.h"
#include "../Oscillator.h"
#include "../Oscillators/SineOscillator.h"
#include "../ParameterTypes/FloatPluginParameter.h"
#include "../ParameterTypes/WaveshapePluginParameter.h"
#include "../ParameterTypes/FloatSetterPluginParameter.h"

class RingModulator : public Plugin {
    public:
        RingModulator();
        void applyWet(float *out);
        void setWaveshape(std::string name);

    private:
        std::unique_ptr<Oscillator> oscillator;
        float frequency;
};


#endif //MADRAS_RINGMODULATOR_H
