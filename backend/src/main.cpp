#include <iostream>
#include <csignal>
#include <cstring>
#include <thread>

#include "AudioEngine.h"
#include "REST/RESTApi.h"

// Messy but need access from signal handler
AudioEngine ae;
RESTApi rest(ae.pluginChain);

void signalHandler(__attribute__((unused))int sig) {
    rest.stop();
    ae.stopStream();
}

int main() {
    std::cout << "My Arm Digital Real-time Audio System" << std::endl;
    // Set up signit
    signal(SIGINT, signalHandler);

    rest.listen();
    ae.audioInit();
    ae.wait();
    std::cout << "Quitting" << std::endl;
    return 0;
}
