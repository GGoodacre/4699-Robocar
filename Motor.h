#pragma once
#include "Component.h"
#include "HBridge.h"

class Motor :
    public Component
{
    public:
        Motor(  CControl& device, 
                std::mutex& lock,
                bool orientation,
                int port_IN1,
                int port_IN2,
                int port_STBY,
                int port_PWM,
                );
        ~Motor();

        set_power(int power);
        set_direction(int direction);
        set_max(int max);

    private:
        int _max_power;
        bool _orientation;
        int _direction;
        int _desired_power;
        int _current_power;
        HBridge _hbridge;

}

