//
// Created by liam on 12/09/15.
//

#ifndef MADRAS_PLUGINPARAMETER_H
#define MADRAS_PLUGINPARAMETER_H

#include <string>
#include <sstream>

class PluginParameter {
public:
    std::string name;
    std::string description;

    virtual std::string getValue() = 0;
    virtual void setValue(std::string val) = 0;
};


#endif //MADRAS_PLUGINPARAMETER_H