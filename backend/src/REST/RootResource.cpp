//
// Created by liam on 13/09/15.
//

#include "RootResource.h"

void RootResource::render(const http_request &request, http_response **response) {
    // Execute the appropriate handler function depending on the path or return a 404
    std::string path = request.get_path();
    std::string method = request.get_method();

    if (this->paths.find(path) != this->paths.end()) {
        // Found path - do we have an entry for the HTTP method being used?
        if (this->paths[path].find(method) != this->paths[path].end()) {
            // Found method so execute it and return
            // Sorry - this is dirty!
            (this->*(this->paths[path][method]))(request, response);
            return;
        }
    }

    // If we fall through to here then we didn't find anything - return 404
    *response = new http_response(http_response_builder("Not Found", 404).string_response());
}

void RootResource::listAvailablePlugins(__attribute__((unused))const http_request &request, http_response **response) {
    auto plugins = this->pluginChain->listAvailablePlugins();

    Json::Value jsonRoot;

    for (auto &plugin : plugins) {
        auto instance = PluginChain::pluginFromString(plugin);
        jsonRoot["plugins"][plugin]["description"] = instance->description;
        for (auto &param : instance->parameters) {
            jsonRoot["plugins"][plugin]["parameters"][param->name];
            jsonRoot["plugins"][plugin]["parameters"][param->name]["description"] = param->description;
        }
    }

    auto writer = Json::StyledWriter();
    auto out = writer.write(jsonRoot);
    *response = new http_response(http_response_builder(out, 200).string_response());
}

void RootResource::listActivePlugins(__attribute__((unused))const http_request &request, http_response **response) {
    Json::Value jsonRoot;
    jsonRoot["plugins"];

    for (unsigned int i = 0; i != this->pluginChain->plugins.size(); i++) {
        auto instance = this->pluginChain->plugins[i].get();
        auto id = std::to_string(i);
        jsonRoot["plugins"][id]["description"] = instance->description;
        jsonRoot["plugins"][id]["type"] = instance->name;
        jsonRoot["plugins"][id]["parameters"];
        for (auto &param : instance->parameters) {
            jsonRoot["plugins"][id]["parameters"][param->name];
            jsonRoot["plugins"][id]["parameters"][param->name]["description"] = param->description;
            jsonRoot["plugins"][id]["parameters"][param->name]["value"] = param->getValue();
        }
    }

    auto writer = Json::StyledWriter();
    auto out = writer.write(jsonRoot);
    *response = new http_response(http_response_builder(out, 200).string_response());
}

void RootResource::getPlugin(const http_request &request, http_response **response) {
    int id = this->idHelper(request, response);
    if (id < 0) { return; }
    auto instance = this->pluginChain->getByID(id);

    Json::Value jsonRoot;
    jsonRoot["plugin"];
    jsonRoot["plugin"]["description"] = instance->description;
    jsonRoot["plugin"]["type"] = instance->name;
    jsonRoot["plugin"]["parameters"];
    for (auto &param : instance->parameters) {
        jsonRoot["plugin"]["parameters"][param->name];
        jsonRoot["plugin"]["parameters"][param->name]["description"] = param->description;
        jsonRoot["plugin"]["parameters"][param->name]["value"] = param->getValue();
    }

    auto writer = Json::StyledWriter();
    auto out = writer.write(jsonRoot);
    *response = new http_response(http_response_builder(out, 200).string_response());
}

void RootResource::addPlugin(const http_request &request, http_response **response) {
    auto pluginName = request.get_arg("name");
    if (pluginName == "") {
        *response = new http_response(http_response_builder("Bad Request", 400).string_response());
        return;
    }

    this->pluginChain->addFromString(pluginName);

    *response = new http_response(http_response_builder("OK", 200).string_response());
}

void RootResource::deletePlugin(const http_request &request, http_response **response) {
    int id = this->idHelper(request, response);
    if (id < 0) { return; }

    this->pluginChain->deleteByID(id);

    *response = new http_response(http_response_builder("OK", 200).string_response());
}

int RootResource::idHelper(const http_request &request, http_response **response) {
    int id = -1;
    try {
        id = stoi(request.get_arg("id"));
    } catch (std::exception &e) {}

    if (id < 0) {
        *response = new http_response(http_response_builder("Bad Request", 400).string_response());
    }

    return id;
}

void RootResource::updatePlugin(const http_request &request, http_response **response) {
    int id = this->idHelper(request, response);
    if (id < 0) { return; }

    auto parameter = request.get_arg("parameter");
    auto value = request.get_arg("value");

    try {
        this->pluginChain->updateByID(id, parameter, value);
    } catch (std::exception &e) {
        std::stringstream s;
        s << "Bad Request: " << e.what();
        *response = new http_response(http_response_builder(s.str(), 400).string_response());
        return;
    }

    *response = new http_response(http_response_builder("OK", 200).string_response());
}
