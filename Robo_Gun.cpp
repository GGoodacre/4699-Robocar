#include "Robo_Gun.h"
#include <thread>
#include <iostream>

Robo_Gun::Robo_Gun() :
    _servo_X(Servo(SERVO_X)),
    _servo_Y(Servo(SERVO_Y)),
    _servo_TRIGGER(Servo(SERVO_TRIGGER)),
    _firing(0)
{
    absolute_move(1500, 500);
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

void Robo_Gun::auto_move(double x_change, double absolute_y)
{
    //std::cout << "Current angle: " << _x;
    _x = _x + x_change*2000/180;
    //std::cout << " New angle: " << _x << std::endl;
    if(_x > 2000) _x = 2000;
    if(_x < 1000) _x = 1000;
    _y = absolute_y*5.556 + 1000;
    //std::cout << "Current angle: " << _y << std::endl;
    if(_y > 2000) _y = 2000;
    if(_y < 1000) _y = 1000;
    _servo_X.set_value(_x);
    _servo_Y.set_value(_y);
}

void Robo_Gun::fire_process(Robo_Gun *ptr)
{
    ptr->_servo_TRIGGER.set_value(1000);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    ptr->_servo_TRIGGER.set_value(2000);
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    ptr->_firing = false;
}

int Robo_Gun::degree2servo(double degree)
{
    return int(degree*(2000/90) + 1500);
}
