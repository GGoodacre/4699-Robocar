#include "Drive.h"

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
    _current_angle(0)
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
    switch(angle) {
    case (angle >= 0 && angle <= 90):
        _motor_left.set_direction(FORWARD);
        _motor_left.set_power(MAX_POWER);
        sin_power = MAX_POWER*cos(deg2rad(angle)*2);
        if(sin_power < 0)
        {
            _motor_right.set_direction(REVERSE);
        }
        else
        {
            _motor_right.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
        break;
    case (angle >= -90 && angle < 0):
        _motor_right.set_direction(FORWARD);
        _motor_right.set_power(MAX_POWER);
        sin_power = MAX_POWER*cos(deg2rad(angle)*2);
        if(sin_power < 0)
        {
            _motor_left.set_direction(REVERSE);
        }
        else
        {
            _motor_left.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
        break;
    case (angle > 90 && angle <= 180):
        _motor_left.set_direction(REVERSE);
        _motor_left.set_power(MAX_POWER);
        sin_power = MAX_POWER*cos(deg2rad(angle)*2 - pi());
        if(sin_power < 0)
        {
            _motor_right.set_direction(REVERSE);
        }
        else
        {
            _motor_right.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
        break;
    case (angle > -180 && angle < -90):
        _motor_right.set_direction(REVERSE);
        _motor_right.set_power(MAX_POWER);
        sin_power = MAX_POWER*cos(deg2rad(angle)*2 - pi());
        if(sin_power < 0)
        {
            _motor_left.set_direction(REVERSE);
        }
        else
        {
            _motor_left.set_direction(FORWARD);
        }
        _motor_right.set_power(abs(sin_power));
        break;
    }
    return;
}

