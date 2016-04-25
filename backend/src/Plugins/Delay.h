//
// Created by liam on 23/10/15.
//

#ifndef MADRAS_DELAY_H
#define MADRAS_DELAY_H

#include "../Plugin.h"
#include "../ParameterTypes/FloatSetterBase.h"
#include "../PluginComponents/DelayLine.h"

class Delay : public Plugin, public FloatSetterBase {
public:
    Delay();

    void applyWet(float *out);
    void setDelay(float delayMS);
    void setFeedback(float feedback);

private:
    // Circular buffer
    std::unique_ptr<DelayLine> delayLine;

    // Positions and delay parameters
    float delayForMS;
    unsigned int delayFor;
    const float maxDelayMS = 2000.0f;
    float feedback;

};


#endif //MADRAS_DELAY_H
