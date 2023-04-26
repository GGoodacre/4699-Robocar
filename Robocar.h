#pragma once
#include "CBase4618.h"
#include "Motor.h"


class Robocar :
    public CBase4618
{
    public:
        Robocar();
        virtual ~Robocar();

        // Update the cars state and handles communication
        void update();

        // 
        void draw();
    private:

        void move();
        std::mutex _gpio_lock;

        Motor _motor_left;
        Motor _motor_right;

        cv::Mat _input_box;
}