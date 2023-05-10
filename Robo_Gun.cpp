#include "Robo_Gun.h"

Robo_Gun::Robo_Gun() : 
    _servo_X(Servo(SERVO_X)),
    _servo_Y(Servo(SERVO_Y)),
    _servo_TRIGGER(Servo(SERVO_TRIGGER))
    _firing(0),
{
    aboslute_move(0, 0);
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
    if(_x > 180) _x = 180;
    if(_x < 0) _x = 0;
    if(_y > 180) _y = 180;
    if(_y < 0) _y = 0;
    _servo_X.set_value(_x);
    _servo_Y.set_value(_y);
}

void Robo_Gun::absolute_move(int x, int y)
{
    _x = x + 90;
    _y = y + 90;
    _servo_X.set_value(_x);
    _servo_Y.set_value(_y);
}

void fire_process(Robo_Gun *ptr)
{
    ptr->_servo_TRIGGER.set_value(120);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    ptr->_servo_TRIGGER.set_value(20);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    _firing = false;
}