#include "Component.h"

Component::init(CControl& device, std::mutex& lock, int type, int port, bool output, bool pullup)
{
    _type = type;
    _port = port;
    _device = device;
    _lock = lock;

    _lock.lock();
    _device.gpio_init(_port, output, pullup);
    _lock.unlock();
}

Component::set(int value)
{
    _lock->lock();
    _device->set_data(_type, _port, value);
    _lock->unlock();
}