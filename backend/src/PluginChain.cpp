//
// Created by liam on 05/09/15.
//

#include "PluginChain.h"

void PluginChain::apply(const float *in, void *midiBuffer, float *out) {
    // Copy input to output
    for (int i = 0; i < StreamParameters::BUFFER_SIZE; i++) {
        out[i] = in[i];
    }

    // Apply each plugin in the chain to the output
    this->deleteLock.lock();
    for (unsigned int i = 0; i != this->plugins.size(); i++) {
        // Copy previous effect's output to wet buffer to get a 100% wet mix
        for (int j = 0; j < StreamParameters::BUFFER_SIZE; j++) {
            this->wetBuffer[j] = out[j];
        }

        // Hand over the midi buffer pointer to anything that needs it
        plugins[i]->setMidiBuffer(midiBuffer);

        // Apply any input gain that needs to be applied
        plugins[i]->applyInputGain(this->wetBuffer);

        // Apply the effect to the wet buffer generating a 100% wet mix
        plugins[i]->applyWet(this->wetBuffer);

        // Mix the wet buffer with the previous effect's output at using the mix amount
        plugins[i]->applyMix(this->wetBuffer, out);
    }
    this->deleteLock.unlock();
}

void PluginChain::addFromString(std::string name) {
    this->plugins.push_back(this->pluginFromString(name));
}

std::shared_ptr<Plugin> PluginChain::getByID(int id) {
    if ((id + 1) > static_cast<int>(this->plugins.size())) {
        throw std::range_error("Plugin index out of range");
    }
    return this->plugins[id];
}

void PluginChain::deleteByID(int id) {
    if ((id + 1) > static_cast<int>(this->plugins.size())) {
        throw std::range_error("Plugin index out of range");
    }
    this->deleteLock.lock();
    this->plugins.erase(this->plugins.begin() + id);
    this->deleteLock.unlock();
}

void PluginChain::updateByID(int id, std::string parameter, std::string value) {
    if ((id + 1) > static_cast<int>(this->plugins.size())) {
        throw std::range_error("Plugin index out of range");
    }

    // Any errors here will be caught higher up
    this->plugins[id]->setParameter(parameter, value);
}