#include "GPIOOutput.h"

GPIOOutput::GPIOOutput(
                        int port )
{
    init(
            DIGITAL,
            port);
}

GPIOOutput::~GPIOOutput()
{
}
