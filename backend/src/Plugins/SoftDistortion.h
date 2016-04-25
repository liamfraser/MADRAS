//
// Created by liam on 20/10/15.
//

#ifndef MADRAS_SOFTDISTORTION_H
#define MADRAS_SOFTDISTORTION_H

#include "../Plugin.h"

class SoftDistortion : public Plugin {
    public:
        SoftDistortion();
        void applyWet(float *out);

    private:
        inline float CubicAmplifier(float input);
};


#endif //MADRAS_SOFTDISTORTION_H
