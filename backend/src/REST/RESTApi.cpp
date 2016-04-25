//
// Created by liam on 12/09/15.
//

#include "RESTApi.h"

RESTApi::RESTApi(std::shared_ptr<PluginChain> pluginChain) {
    this->ws = std::unique_ptr<webserver>(new webserver(create_webserver(8080).max_threads(5)));
    this->rootResource = std::unique_ptr<RootResource>(new RootResource(pluginChain));
    this->ws->register_resource("/", this->rootResource.get(), true);
}

void RESTApi::listen() {
    // Start a non-blocking web server
    this->ws->start(false);
}

void RESTApi::stop() {
    this->ws->stop();
}