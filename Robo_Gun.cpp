#include "Robo_Gun.h"
#include <thread>

Robo_Gun::Robo_Gun() :
    _servo_X(Servo(SERVO_X)),
    _servo_Y(Servo(SERVO_Y)),
    _servo_TRIGGER(Servo(SERVO_TRIGGER)),
    _firing(0)
{
    absolute_move(1500, 1500);
}

bool Robo_Gun::fire()
{
    if(_firing)
    {
        return false;
    }
    else
    {
        std::thread th_fire(&Robo_Gun::fire_process, this);
        th_fire.detach();
        _firing = true;
        return true;
    }
}

void Robo_Gun::relative_move(int x, int y)
{
    _x = _x + x;
    _y = _y + y;
    if(_x > 2000) _x = 2000;
    if(_x < 1000) _x = 1000;
    if(_y > 2000) _y = 2000;
    if(_y < 1000) _y = 1000;
    _servo_X.set_value(_x);
    _servo_Y.set_value(_y);
}

void Robo_Gun::absolute_move(int x, int y)
{
    _x = x;
    _y = y;
    _servo_X.set_value(_x);
    _servo_Y.set_value(_y);
}

void Robo_Gun::fire_process(Robo_Gun *ptr)
{
    ptr->_servo_TRIGGER.set_value(1800);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    ptr->_servo_TRIGGER.set_value(1200);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    ptr->_firing = false;
}
