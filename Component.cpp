#include "Component.h"

void Component::init(int type, int port, bool output, bool pullup)
{
    _type = type;
    _port = port;

    _device.gpio_init(_port, output, pullup);
}

void Component::set_value(int value)
{
    _device.set_data(_type, _port, value);
}

Component::~Component() {

}
