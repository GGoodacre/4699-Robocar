#pragma once
#include "Servo.h"

class Robo_Gun 
{
    public:
        Robo_Gun();

        bool fire();
        void relative_move(int x, int y);
        void absolute_move(int x, int y);
    private:

        Servo _servo_X;
        Servo _servo_Y;
        Servo _servo_TRIGGER;

        int _x;
        int _y;

        bool _firing;

        void fire_process(Robo_Gun* ptr);
};

