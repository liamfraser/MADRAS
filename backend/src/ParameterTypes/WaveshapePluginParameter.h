//
// Created by liam on 12/09/15.
//

#ifndef MADRAS_WAVESHAPEPLUGINPARAMETER_H
#define MADRAS_WAVESHAPEPLUGINPARAMETER_H

#include <memory>

#include "../PluginParameter.h"

// Forward declaration
class RingModulator;

// Not fully implemented

class WaveshapePluginParameter : public PluginParameter {
public:
    WaveshapePluginParameter(std::string name, std::string description, std::string waveshape, RingModulator *setterObj,
                             void (RingModulator::*setter)(std::string)) {
        this->name = name;
        this->description = description;
        this->waveshape = waveshape;
        this->setterObj = setterObj;
        this->setter = setter;
    }

    std::string getValue() {
        return this->waveshape;
    }

    void setValue(std::string val) {
        if (val != "sine") {
            std::stringstream s;
            s << "Invalid waveshape " << val;
            throw std::invalid_argument(s.str());
        }

        this->waveshape = name;
        (setterObj->*setter)(name);
    }

private:
    std::string waveshape;
    RingModulator *setterObj = nullptr;

    void (RingModulator::*setter)(std::string) = nullptr;
};

#endif //MADRAS_WAVESHAPEPLUGINPARAMETER_H
