#include "Drive.h"
#include <math.h>
#include <thread>
#include <iostream>

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
    else if(angle > 45 && angle <= 135)
    {
        _motor_right.set_direction(REVERSE);
        _motor_right.set_power(_max_power);
        _motor_left.set_direction(FORWARD);
        _motor_left.set_power(_max_power);
    }
    else if(angle < -45 && angle >= -135)
    {
        _motor_right.set_direction(FORWARD);
        _motor_right.set_power(_max_power);
        _motor_left.set_direction(REVERSE);
        _motor_left.set_power(_max_power);
    }
    return;
}

void Drive::set_max_power(int power)
{
    if(_running == false)
    {
        _max_power = power;
    }
}


void Drive::go_until(int angle, double distance)
{
    std::chrono::microseconds time_to_run;
    if(angle > 180)
    {
        angle = angle - 360;
    }
    if(angle < -180)
    {
        angle = angle + 360;
    }

    distance = abs(distance);

    if(angle >= -45 && angle <= 45)
    {
        time_to_run = std::chrono::microseconds(int(distance*DRIVE_SPEED*10000*_max_power));
    }
    else if(angle >= 135 && angle <= 180)
    {
        time_to_run = std::chrono::microseconds(int(distance*DRIVE_SPEED*10000*_max_power));
    }
    else if(angle > -180 && angle <= -135)
    {
        time_to_run = std::chrono::microseconds(int(distance*DRIVE_SPEED*10000*_max_power));
    }
    else if(angle > 45 && angle <= 135)
    {
        time_to_run = std::chrono::microseconds(int(abs(angle)*TURN_SPEED*10000*_max_power));
    }
    else if(angle < -45 && angle >= -135)
    {
        time_to_run = std::chrono::microseconds(int(abs(angle)*TURN_SPEED*10000*_max_power));
    }
    if(_running)
    {
        _lock.lock();
        _current_angle = angle;
        _end_time = std::chrono::system_clock::now() + time_to_run;
        _lock.unlock();
    }
    else
    {
        _running = true;
        _current_angle = angle;
        _end_time = std::chrono::system_clock::now() + time_to_run;
        std::thread motor(Drive::run_motor, this);
        motor.detach();
    }
}

void Drive::run_motor(Drive *ptr)
{
    int time_left;
    do
    {
        ptr->_lock.lock();
        //std::cout << "RUNNING" << std::endl;
        ptr->set_direction(ptr->_current_angle);
        time_left = std::chrono::duration_cast<std::chrono::milliseconds>(ptr->_end_time - std::chrono::system_clock::now()).count();
        //std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(ptr->_end_time - std::chrono::system_clock::now()).count() << std::endl;
        ptr->_lock.unlock();
    } while (time_left > 0);
    ptr->stop();
    ptr->_running = false;
}
