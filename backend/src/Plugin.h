//
// Created by liam on 05/09/15.
//

#ifndef MADRAS_PLUGIN_H
#define MADRAS_PLUGIN_H

#include <vector>
#include <memory>
#include <string>

#include "PluginParameter.h"
#include "ParameterTypes/FloatPluginParameter.h"
#include "StreamParameters.h"

class Plugin {
public:
    Plugin() {
        this->mix = 1.0;
        this->inputGain = 1.0;
        this->outputGain = 1.0;
        this->addParameter(new FloatPluginParameter("mix", "Mix of effect to output signal",
                                                    &this->mix));
        this->addParameter(new FloatPluginParameter("inputGain", "Amount to amplify input signal",
                                                    &this->inputGain, 0.0, 10.0));
        this->addParameter(new FloatPluginParameter("outputGain", "Amount to amplify output signal",
                                                    &this->outputGain, 0.0, 10.0));
    }

    virtual void applyWet(float *out) = 0;

    void setMidiBuffer(void *midiBuffer) {
        this->midiBuffer = midiBuffer;
    }

    void applyInputGain(float *out) {
        // Called before apply wet
        for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
            out[i] = out[i] * inputGain;
        }
    }

    void applyMix(float *wetBuffer, float *out) {
        // Mix the wet buffer and previous effect's output
        float wet = this->mix;
        float dry = 1.0f - this->mix;

        for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
            out[i] = (out[i] * dry) + (wetBuffer[i] * wet);
            out[i] = out[i] * this->outputGain;
        }
    }

    std::string description;
    std::string name;
    std::vector<std::shared_ptr<PluginParameter>> parameters;

    void setParameter(std::string name, std::string value) {
        for (auto p : this->parameters) {
            if (p->name == name) {
                p->setValue(value);
                return;
            }
        }
        throw std::invalid_argument("Invalid parameter name");
    }

    void addParameter(PluginParameter *p) {
        this->parameters.push_back(std::shared_ptr<PluginParameter>(p));
    }

protected:
    float mix;
    float inputGain;
    float outputGain;
    void *midiBuffer;
};

#endif //MADRAS_PLUGIN_H
