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
        int _desired_power;
        int _desired_angle;

        std::chrono::time_point<std::chrono::system_clock> _end_time;
        std::chrono::time_point<std::chrono::system_clock> _turn_time;
        int _run_direction;

        static void run_motor(Drive* ptr);

        std::mutex _lock;
        bool _running;

};

#define MAX_POWER 100
#define TURN_SPEED  0.0020
#define DRIVE_SPEED 0.7
#define CAR_WIDTH 0.15