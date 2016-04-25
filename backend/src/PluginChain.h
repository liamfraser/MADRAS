//
// Created by liam on 05/09/15.
//

#ifndef MADRAS_PLUGINCHAIN_H
#define MADRAS_PLUGINCHAIN_H

#include <vector>
#include <memory>
#include <string>
#include <mutex>

#include "Plugin.h"
#include "StreamParameters.h"
#include "Plugins/RingModulator.h"
#include "Plugins/OscillatorInsert.h"
#include "Plugins/BiQuad/LowPass.h"
#include "Plugins/BiQuad/HighPass.h"
#include "Plugins/PitchShifters/PitchShift.h"
#include "Plugins/SoftDistortion.h"
#include "Plugins/Delay.h"
#include "Plugins/Flanger.h"
#include "Plugins/PitchShifters/Vocoder.h"
#include "Plugins/Chorus.h"
#include "Plugins/Reverb.h"

class PluginChain {
public:
    PluginChain() {
        this->wetBuffer = new float[StreamParameters::BUFFER_SIZE];
    }

    ~PluginChain() {
        delete this->wetBuffer;
    }

    void apply(const float *in, void *midiBuffer, float *out);
    void addFromString(std::string name);
    void deleteByID(int id);
    void updateByID(int id, std::string parameter, std::string value);
    std::shared_ptr<Plugin> getByID(int id);

    std::vector<std::string> listAvailablePlugins() {
        return this->availablePlugins;
    }

    static std::shared_ptr<Plugin> pluginFromString(std::string name) {
        if (name == "RingModulator") {
            return std::unique_ptr<Plugin>(new RingModulator());
        } else if (name == "OscillatorInsert") {
            return std::unique_ptr<Plugin>(new OscillatorInsert());
        } else if (name == "LowPass") {
            return std::unique_ptr<Plugin>(new LowPass());
        } else if (name == "HighPass") {
            return std::unique_ptr<Plugin>(new HighPass());
        } else if (name == "PitchShift") {
            return std::unique_ptr<Plugin>(new PitchShift());
        } else if (name == "SoftDistortion") {
            return std::unique_ptr<Plugin>(new SoftDistortion());
        } else if (name == "Delay") {
            return std::unique_ptr<Plugin>(new Delay());
        } else if (name == "Flanger") {
            return std::unique_ptr<Plugin>(new Flanger());
        } else if (name == "Chorus") {
            return std::unique_ptr<Plugin>(new Chorus());
        } else if (name == "Vocoder") {
            return std::unique_ptr<Plugin>(new Vocoder());
        } else if (name == "Reverb") {
            return std::unique_ptr<Plugin>(new Reverb());
        } else {
            throw std::invalid_argument("Invalid plugin type");
        }
    }

    std::vector<std::shared_ptr<Plugin>> plugins;
    std::mutex deleteLock;

private:
    const std::vector<std::string> availablePlugins = {"RingModulator", "OscillatorInsert", "LowPass", "HighPass",
                                                       "SoftDistortion", "Delay", "Flanger", "Chorus", "Reverb"};
    float *wetBuffer = nullptr;
};

#endif //MADRAS_PLUGINCHAIN_H
