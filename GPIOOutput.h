#pragma once
#include "Component.h"

class GPIOOutput :
    public Component
{
    public:
        GPIOOutput( CControl& device,
                    std::mutex& lock,
                    int port
                    );
        GPIOOutput();

        ~GPIOOutput();
};

#define HIGH 1
#define LOW 0
