//
// Created by liam on 12/09/15.
//

#ifndef MADRAS_RESTAPI_H
#define MADRAS_RESTAPI_H

#include <memory>

#include "../PluginChain.h"
#include "RootResource.h"

#include "httpserver.hpp"
using namespace httpserver;

class RESTApi {
    public:
        RESTApi(std::shared_ptr<PluginChain> pluginChain);
        void listen();
        void stop();

    private:
        std::unique_ptr<webserver> ws;
        std::unique_ptr<RootResource> rootResource;
};


#endif //MADRAS_RESTAPI_H
