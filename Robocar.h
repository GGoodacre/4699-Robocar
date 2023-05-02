#pragma once
#include "CBase4618.h"
#include "Motor.h"
#include "cvui.h"
#include "Drive.h"
#include "Robo_Server.h"
#include "Robo_Client.h"
#include "Camera.h"

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

        // ROBOT CONTROL
        Drive _drive;
        cv::Mat _input_box;

        int _mode;
        cv::Mat _pi_camera;

        Robo_Server _server;
        Camera _camera;
        void drivePI();
        void testPI();
};

enum {STANDBY = 0, PI, MANUAL, AUTO};
#define STANDBY_MODE '~'
#define INPUT_WIDTH 200
#define INPUT_HEIGHT 200
