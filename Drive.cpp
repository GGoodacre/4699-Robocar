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
    std::chrono::microseconds time_to_turn;
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

        double v1 = 100;
        double v2 = abs(_max_power*cos(angle*M_PI*4/180));
        double radius = abs(((v2+v1)/(v2-v1))*CAR_WIDTH);
        double turn_distance = radius*(angle*(M_PI/180));
        double distance_travelled = radius*sin(angle*(M_PI/180));
        double run_distance = distance - turn_distance;

        time_to_run = std::chrono::microseconds(int(run_distance*DRIVE_SPEED*10000*_max_power));
        time_to_turn = std::chrono::microseconds(int(turn_distance*DRIVE_SPEED*10000*_max_power));
        _run_direction = 0;

    }
    else if(angle >= 135 && angle <= 180)
    {
        double v1 = 100;
        double v2 = abs(_max_power*cos(angle*M_PI*4/180));
        double radius = abs(((v2+v1)/(v2-v1))*CAR_WIDTH);
        double turn_distance = radius*(angle*(M_PI/180));
        double distance_travelled = radius*sin((angle-180)*(M_PI/180));
        double run_distance = distance - turn_distance;

        time_to_run = std::chrono::microseconds(int(run_distance*DRIVE_SPEED*10000*_max_power));
        time_to_turn = std::chrono::microseconds(int(turn_distance*DRIVE_SPEED*10000*_max_power));
        _run_direction = 180;

    }
    else if(angle > -180 && angle <= -135)
    {
        double v1 = 100;
        double v2 = abs(_max_power*cos(angle*M_PI*4/180));
        double radius = abs(((v2+v1)/(v2-v1))*CAR_WIDTH);
        double turn_distance = radius*(angle*(M_PI/180));
        double distance_travelled = radius*sin((angle+180)*(M_PI/180));
        double run_distance = distance - turn_distance;

        time_to_run = std::chrono::microseconds(int(run_distance*DRIVE_SPEED*10000*_max_power));
        time_to_turn = std::chrono::microseconds(int(turn_distance*DRIVE_SPEED*10000*_max_power));
        _run_direction = 180;

    }
    else if(angle > 45 && angle < 135)
    {
        time_to_run = std::chrono::microseconds(int(distance*DRIVE_SPEED*10000*_max_power));
        time_to_turn = std::chrono::microseconds(int(abs(angle)*TURN_SPEED*10000*_max_power));
        _run_direction = 0;
    }
    else if(angle < -45 && angle > -135)
    {
        time_to_run = std::chrono::microseconds(int(distance*DRIVE_SPEED*10000*_max_power));
        time_to_turn = std::chrono::microseconds(int(abs(angle)*TURN_SPEED*10000*_max_power));
        _run_direction = 0;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////

    if(_running)
    {
        _lock.lock();
        _desired_angle = angle;
        _turn_time = std::chrono::system_clock::now() + time_to_turn;
        _end_time = std::chrono::system_clock::now() + time_to_run + time_to_turn;
        _lock.unlock();
    }
    else
    {
        _running = true;
        _desired_angle = angle;
        _turn_time = std::chrono::system_clock::now() + time_to_turn;
        _end_time = std::chrono::system_clock::now() + time_to_run + time_to_turn;
        std::thread motor(Drive::run_motor, this);
        motor.detach();
    }
}

void Drive::run_motor(Drive *ptr)
{
    int time_left;
    std::chrono::time_point<std::chrono::system_clock> _max_power_time = std::chrono::system_clock::now() + std::chrono::milliseconds(50);
    _desired_power = _max_power;
    _max_power = 100;
    do
    {
        ptr->_lock.lock();

        if(std::chrono::duration_cast<std::chrono::milliseconds>(ptr->_turn_time - std::chrono::system_clock::now()).count() > 0)
        {
            ptr->set_direction(ptr->_desired_angle);
        }
        else
        {
            ptr->set_direction(ptr->_run_direction);
        }
        
        time_left = std::chrono::duration_cast<std::chrono::milliseconds>(ptr->_end_time - std::chrono::system_clock::now()).count();

        if(std::chrono::duration_cast<std::chrono::milliseconds>(ptr->_end_time - std::chrono::system_clock::now()).count() <= 0)
        {
            _max_power = _desired_power;
        }
        ptr->_lock.unlock();
    } while (time_left > 0);
    ptr->stop();
    ptr->_running = false;
}
