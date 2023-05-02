#pragma once
#include "Motor.h"

class Drive
{
    public:
        Drive();

        void start();
        void stop();
        void set_direction(int angle);

    private:

        Motor _motor_left;
        Motor _motor_right;

        int _current_angle;


};

#define MAX_POWER 100
