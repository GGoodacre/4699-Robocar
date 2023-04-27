#include "GPIOOutput.h"

GPIOOutput::GPIOOutput( CControl& device,
                        std::mutex& lock,
                        int port )
{
    init(   device,
            lock,
            DIGITAL,
            port);
}
