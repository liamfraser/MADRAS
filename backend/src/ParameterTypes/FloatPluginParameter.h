//
// Created by liam on 12/09/15.
//

#ifndef MADRAS_FLOATPLUGINPARAMETER_H
#define MADRAS_FLOATPLUGINPARAMETER_H

#include "../PluginParameter.h"


class FloatPluginParameter : public PluginParameter {
public:
    FloatPluginParameter(std::string name, std::string description, float *value, float minVal = 0, float maxVal = 1) {
        this->name = name;
        this->description = description;
        this->value = value;
        this->minVal = minVal;
        this->maxVal = maxVal;
    }

    float checkValid(std::string &val) {
        // If there is an exception in this it will be caught above
        float f = std::stof(val);

        if (f < this->minVal || f > this->maxVal) {
            std::stringstream s;
            s << "Float value " << f << " not in range " << this->minVal << " < f < " << this->maxVal;
            throw std::range_error(s.str());
        }
        return f;
    }

    void setValue(std::string val) {
        auto f = this->checkValid(val);
        *this->value = f;
    }

    std::string getValue() {
        return std::to_string(*this->value);
    }

protected:
    float minVal;
    float maxVal;
    float *value = nullptr;
};


#endif //MADRAS_FLOATPLUGINPARAMETER_H
