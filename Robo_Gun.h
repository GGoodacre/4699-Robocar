#pragma once
#include "Servo.h"

class Robo_Gun
{
    public:
        Robo_Gun();

        bool fire();
        void relative_move(int x, int y);
        void absolute_move(int x, int y);
        void auto_move(double x_change, double absolute_y);
        int degree2servo(double degree);
        int get_y() {return _y; };

    private:

        Servo _servo_X;
        Servo _servo_Y;
        Servo _servo_TRIGGER;

        int _x;
        int _y;

        bool _firing;

        static void fire_process(Robo_Gun* ptr);
};

