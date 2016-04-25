//
// Created by liam on 29/08/15.
//

#include "AudioEngine.h"

// Jack stuff based on
// https://github.com/jackaudio/jack2/blob/master/example-clients/thru_client.c

AudioEngine::AudioEngine() {
    this->pluginChain = std::unique_ptr<PluginChain>(new PluginChain());
}

void AudioEngine::audioInit() {
    // Attempt to connect to jack server
    jack_options_t options = JackNullOption;
    this->client = jack_client_open("madras", options, NULL);
    if (this->client == nullptr) {
        throw std::runtime_error("Unable to connect to Jack server");
    }

    // Get jack to call us when we have to process data
    jack_set_process_callback(this->client, nonMemberJackCallback, this);

    // And also when it is shutdown
    jack_on_shutdown(this->client, nonMemberJackShutdown, this);

    // Register ports with jack
    this->outputPort = jack_port_register(this->client, "output", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    this->inputPort = jack_port_register(this->client, "input", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
    this->midiInputPort = jack_port_register(this->client, "midiInput", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);

    if (jack_activate(this->client)) {
        throw std::runtime_error("Cannot activate client");
    }

    // Attempt to connect ports
    if (jack_connect(this->client, "system:capture_1", "madras:input")) {
        throw std::runtime_error("Cannot connect system:capture_1 to madras:input");
    }

    if (jack_connect(this->client, "madras:output", "system:playback_1")) {
        throw std::runtime_error("Cannot connect madras:output to system:playback_1");
    }

    if (jack_connect(this->client, "system:midi_capture_2", "madras:midiInput")) {
        std::cout << "Couldn't connect to midi" << std::endl;
        this->haveMidi = false;
    }
}

int AudioEngine::jackCallback(jack_nframes_t nframes) {
    // Jack needs us to process a block of audio
    jack_default_audio_sample_t *in, *out;
    in = static_cast<jack_default_audio_sample_t *>(jack_port_get_buffer(this->inputPort, nframes));
    out = static_cast<jack_default_audio_sample_t *>(jack_port_get_buffer(this->outputPort, nframes));

    void *midiBuffer = nullptr;
    if (this->haveMidi) {
        midiBuffer = jack_port_get_buffer(this->midiInputPort, nframes);
    }

    this->pluginChain->apply(in, midiBuffer, out);

    return 0;
}

void AudioEngine::jackShutdown() {
    throw std::runtime_error("Jack shut us down!");
    // Exception will cause us to exit
}

int nonMemberJackCallback(jack_nframes_t nframes, void *arg) {
    assert(nframes == StreamParameters::BUFFER_SIZE);
    return static_cast<AudioEngine*>(arg)->jackCallback(nframes);
}

void nonMemberJackShutdown(void *arg) {
    return static_cast<AudioEngine*>(arg)->jackShutdown();
}

void AudioEngine::stopStream() {
    this->stop = true;
    jack_client_close(this->client);
}

void AudioEngine::wait() {
    while (!this->stop) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
