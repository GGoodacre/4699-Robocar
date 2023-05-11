#pragma once
#include "Motor.h"

class Drive
{
    public:
        Drive();

        void start();
        void stop();
        void set_direction(int angle);
        void set_max_power(int power) { _max_power = power; };

    private:

        Motor _motor_left;
        Motor _motor_right;

        int _current_angle;
        int _max_power;


};

#define MAX_POWER 60
