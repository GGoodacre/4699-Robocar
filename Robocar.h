#pragma once
#include "CBase4618.h"
#include "Motor.h"
#include "cvui.h"


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
};

////////////////////////////////////////////////////////////////////////////
// PARAMETERS
////////////////////////////////////////////////////////////////////////////
#define INPUT_WIDTH 600
#define INPUT_HEIGHT 700
