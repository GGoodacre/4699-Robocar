#include "Motor.h"

Motor::Motor(   CControl &device, 
                std::mutex &lock, 
                bool orientation, 
                int port_IN1, 
                int port_IN2, 
                int port_STBY, 
                int port_PWM, )
{
    _orientation = orientation;
    _direction = BRAKE;

    init(   device,
            lock,
            PWM,
            port_PWM);
    
    _hbridge = HBridge( device,
                        lock,
                        _orientation,
                        port_IN1,
                        port_IN2,
                        port_STBY);
   
    set_max(100);
    set_power(0);
}

Motor::set_power(int power)
{
    this.set((power*_max)/100);
}

Motor::set_direction(int direction)
{
    _hbridge.set_direction(direction);
}

Motor::set_max(int max)
{
    _max = max*PWM_scale;
}

define PWM_scale 255