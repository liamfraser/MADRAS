//
// Created by liam on 29/08/15.
//

#ifndef MADRAS_AUDIOENGINE_H
#define MADRAS_AUDIOENGINE_H

#include <mutex>
#include <thread>
#include <cstring>
#include <cassert>
#include <chrono>
#include <memory>
#include <iostream>
#include <jack/jack.h>

#include "PluginChain.h"
#include "StreamParameters.h"

class AudioEngine {
    public:
        AudioEngine();
        void audioInit();
        int jackCallback(jack_nframes_t nframes);
        void jackShutdown();
        void wait();
        void stopStream();
        std::shared_ptr<PluginChain> pluginChain;

private:
        jack_client_t *client;
        jack_port_t *inputPort;
        jack_port_t *midiInputPort;
        jack_port_t *outputPort;
        bool haveMidi = true;
        bool stop = false;
};

int nonMemberJackCallback(jack_nframes_t nframes, void *arg);
void nonMemberJackShutdown(void *arg);

#endif //MADRAS_AUDIOENGINE_H
