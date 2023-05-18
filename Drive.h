#pragma once
#include "Motor.h"

class Drive
{
    public:
        Drive();

        void start();
        void stop();
        void set_direction(int angle);
        void set_max_power(int power);
        void go_until(int angle, double distance);

    private:

        Motor _motor_left;
        Motor _motor_right;

        int _current_angle;
        int _max_power;

        std::chrono::time_point<std::chrono::system_clock> _end_time;

        static void run_motor(Drive* ptr);

        std::mutex _lock;
        bool _running;

};

#define MAX_POWER 60
#define TURN_SPEED  0.0026388889
#define DRIVE_SPEED 0.9405074366
