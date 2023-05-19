#include "Motor.h"
#include <iostream>

Motor::Motor(
                bool orientation,
                int port_IN1,
                int port_IN2,
                int port_STBY,
                int port_PWM ) :
        _orientation(orientation),
        _direction(BRAKE),
        _hbridge(HBridge(_orientation, port_IN1, port_IN2, port_STBY))
{
    init(
            PWM,
            port_PWM);

    set_max(100);
    set_power(0);
}

void Motor::set_power(int power)
{
    this->set_value((power*_max_power)/100);
}

void Motor::set_direction(int direction)
{
    _hbridge.set_direction(direction);
}

void Motor::set_max(int max)
{
    _max_power = (max*PWM_scale)/100;
}

Motor::~Motor() {
}

