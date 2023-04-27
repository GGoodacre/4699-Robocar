#pragma once
#include "Component.h"

class GPIOOutput :
    public Component
{
    public:
        GPIOOutput(
                    int port
                    );
        ~GPIOOutput();
};

#define HIGH 1
#define LOW 0
