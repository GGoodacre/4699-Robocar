#include "Drive.h"
#include <math.h>

Drive::Drive() :
    _motor_left(Motor(
                            CCW,
                            AIN1,
                            AIN2,
                            STBY,
                            MOTOR_LEFT
                            )),
    _motor_right(Motor(
                            CW,
                            BIN1,
                            BIN2,
                            STBY,
                            MOTOR_RIGHT
                            )),
    _current_angle(0),
    _max_power(MAX_POWER)
{

}

void Drive::start()
{
    set_direction(_current_angle);
}

void Drive::stop()
{
    _motor_right.set_direction(BRAKE);
    _motor_left.set_direction(BRAKE);
}

void Drive::set_direction(int angle)
{
    float sin_power;

    if(angle >= 0 && angle <= 45)
    {
        _motor_left.set_direction(FORWARD);
        _motor_left.set_power(_max_power);
        sin_power = _max_power*cos(angle*M_PI*4/180);
        if(sin_power < 0)
        {
            _motor_right.set_direction(REVERSE);
        }
        else
        {
            _motor_right.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
    }
    else if(angle >= -45 && angle < 0)
    {
        _motor_right.set_direction(FORWARD);
        _motor_right.set_power(_max_power);
        sin_power = _max_power*cos(angle*M_PI*4/180);
        if(sin_power < 0)
        {
            _motor_left.set_direction(REVERSE);
        }
        else
        {
            _motor_left.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
    }
    else if(angle >= 135 && angle <= 180)
    {
        _motor_left.set_direction(REVERSE);
        _motor_left.set_power(_max_power);
        sin_power = _max_power*cos(angle*M_PI*4/180 - M_PI);
        if(sin_power < 0)
        {
            _motor_right.set_direction(REVERSE);
        }
        else
        {
            _motor_right.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
    }
    else if(angle > -180 && angle <= -135)
    {
        _motor_right.set_direction(REVERSE);
        _motor_right.set_power(_max_power);
        sin_power = _max_power*cos(angle*M_PI*4/180 - M_PI);
        if(sin_power < 0)
        {
            _motor_left.set_direction(REVERSE);
        }
        else
        {
            _motor_left.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
    }
    else if(angle > 45 && angle < 135)
    {
        _motor_right.set_direction(REVERSE);
        _motor_right.set_power(_max_power);
        _motor_left.set_direction(FORWARD);
        _motor_left.set_power(_max_power);
    }
    else if(angle < -45 && angle > -135)
    {
        _motor_right.set_direction(FORWARD);
        _motor_right.set_power(_max_power);
        _motor_left.set_direction(REVERSE);
        _motor_left.set_power(_max_power);
    }
    return;
}

