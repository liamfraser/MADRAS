//
// Created by liam on 13/09/15.
//

#ifndef MADRAS_ROOTRESOURCE_H
#define MADRAS_ROOTRESOURCE_H

#include <memory>
#include <string>
#include <map>

#include "../PluginChain.h"

#include "httpserver.hpp"

using namespace httpserver;

#include "json/json.h"

class RootResource : public http_resource {
public:
    RootResource(std::shared_ptr<PluginChain> pluginChain) {
        this->pluginChain = pluginChain;
    }

    // Get ID number from request and validate it
    int idHelper(const http_request &request, http_response **response);

    // REST handlers
    void render(const http_request &, http_response **);
    void listAvailablePlugins(const http_request &, http_response **);
    void listActivePlugins(const http_request &, http_response **);
    void addPlugin(const http_request &, http_response **);
    void deletePlugin(const http_request &, http_response **);
    void getPlugin(const http_request &, http_response **);
    void updatePlugin(const http_request &, http_response **);

private:
    std::shared_ptr<PluginChain> pluginChain;

    // Map of path, to map of request method and appropriate handler function
    std::map<std::string, std::map<std::string, void (RootResource::*)(const http_request &, http_response **)>> paths{
            {"/plugins/all",    {{"GET",  &RootResource::listAvailablePlugins}}},
            {"/plugins/list",   {{"GET",  &RootResource::listActivePlugins}}},
            {"/plugins/add",    {{"POST", &RootResource::addPlugin}}},
            {"/plugins/delete", {{"POST", &RootResource::deletePlugin}}},
            {"/plugins/update", {{"POST", &RootResource::updatePlugin}}},
            {"/plugins/get",    {{"GET",  &RootResource::getPlugin}}}
    };
};


#endif //MADRAS_ROOTRESOURCE_H
