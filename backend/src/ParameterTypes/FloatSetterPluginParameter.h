//
// Created by liam on 12/09/15.
//

#ifndef MADRAS_FLOATSETTERPLUGINPARAMETER_H
#define MADRAS_FLOATSETTERPLUGINPARAMETER_H


#include "FloatPluginParameter.h"
#include "../Oscillator.h"
#include "../Plugin.h"

class FloatSetterPluginParameter : public FloatPluginParameter {
public:
    FloatSetterPluginParameter(std::string name, std::string description, float *value, FloatSetterBase *setterObj,
                               void (FloatSetterBase::*setter)(float), float minVal = 0, float maxVal = 1)
            : FloatPluginParameter(name, description, value, minVal, maxVal) {
        this->setterObj = setterObj;
        this->setter = setter;
    }

    void setValue(std::string val) {
        auto f = this->checkValid(val);
        *this->value = f;
        (this->setterObj->*this->setter)(f);
    }

private:
    FloatSetterBase *setterObj = nullptr;
    void (FloatSetterBase::*setter)(float) = nullptr;
};

#endif //MADRAS_OSCILLATORFREQPLUGINPARAMETER_H
