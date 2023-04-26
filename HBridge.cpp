#include "HBridge.h"

HBridge::Hbridge(   CControl &device, 
                    std::mutex &lock, 
                    bool orientation, 
                    int port_IN1, 
                    int port_IN2, 
                    int port_STBY)
{
    _IN1 = GPIOOutput(device, lock, port_IN1);
    _IN2 = GPIOOutput(device, lock, port_IN2);
    _STBY = GPIOOutput(device, lock, port_STBY);
    _STBY.set(HIGH);

    _orientation = orientation;
    set_direction(BRAKE);
}

void HBridge::set_direction(int direction)
{
    _direction = direction;
    switch(direction) {
        case FORWARD:
            _IN1.set(orientation);
            _IN2.set(~orientation);
            break;
        case REVERSE:
            _IN1.set(~orientation);
            _IN2.set(orientation);
            break;
        case BRAKE:
            _IN1.set(LOW);
            _IN2.set(LOW);
            break;
    }
}