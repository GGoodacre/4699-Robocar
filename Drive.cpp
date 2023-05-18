#include "Drive.h"
#include <math.h>
#include <thread>

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
    if(_lock.try_lock())
    {
        set_direction(_current_angle);
        _lock.unlock();
    }
}

void Drive::stop()
{
    if(_lock.try_lock())
    {
        _motor_right.set_direction(BRAKE);
        _motor_left.set_direction(BRAKE);
        _lock.unlock()
    }
}

void Drive::set_direction(int angle)
{
    if(_lock.try_lock())
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
        _lock.unlock();
    }
}

void Drive::set_max_power(int power)
{
    if(_lock.try_lock)
    {
        _max_power = power;
        _lock.unlock();
    }
}


void Drive::go_until(int angle, double distance)
{
    std::chrono::milliseconds time_to_run;
    if((angle > 22.5) || (angle < -22.5))
    {
        time_to_run = std::chrono::seconds(angle*TURN_SPEED);
    }
    else
    {
        time_to_run = std::chrono::seconds(distance*DRIVE_SPEED);
    }
    if(_running)
    {
        _lock.lock()
        _current_angle = angle;
        _end_time = std::chrono::system_clock::now() + time_to_run;
        _lock.unlock();
    }
    else
    {
        _running = true;
        _current_angle =angle;
        _end_time = std::chrono::system_clock::now() + time_to_run; 
        std::thread motor(Drive::run_moter, this);
        motor.detach();
    }
}

void Drive::run_motor(Drive *ptr)
{
    do
    {
        ptr->set_direction(ptr->_current_angle);
    } while (_end_time > std::chrono::system_clock::now());
    
}
