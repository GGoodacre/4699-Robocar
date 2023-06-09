#include "HBridge.h"

HBridge::HBridge(
                    bool orientation,
                    int port_IN1,
                    int port_IN2,
                    int port_STBY) :
        _IN1(GPIOOutput(port_IN1)),
        _IN2(GPIOOutput(port_IN2)),
        _STBY(GPIOOutput(port_STBY)),
        _orientation(orientation)
{
    _STBY.set_value(HIGH);
    set_direction(BRAKE);
}

void HBridge::set_direction(int direction)
{
    _direction = direction;
    switch(direction) {
        case FORWARD:
            _IN1.set_value(_orientation);
            _IN2.set_value(!_orientation);
            break;
        case REVERSE:
            _IN1.set_value(!_orientation);
            _IN2.set_value(_orientation);
            break;
        case BRAKE:
            _IN1.set_value(LOW);
            _IN2.set_value(LOW);
            break;
    }
}


HBridge::~HBridge() {
}
