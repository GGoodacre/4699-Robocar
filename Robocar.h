#pragma once
#include "CBase4618.h"
#include "Motor.h"
#include "cvui.h"
#include "Drive.h"
#include "Robo_Server.h"
#include "Robo_Client.h"
#include "Camera.h"
#include "Robo_Gun.h"

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

        int _mode;
        std::vector<int> test_ids;
        // ROBOT CONTROL
        Drive _drive;
        Robo_Gun _gun;

        cv::Mat _pi_camera;
        cv::Mat _input_box;

        Robo_Server _server;
        Robo_Client _client;

        Camera _camera;


        void drivePI();
        void testPI();



        void telecommunication_mode();
        void telecommunication_drive(std::string cmd);
        void telecommunication_shoot(std::string cmd);
        std::chrono::system_clock::time_point _time_telecom;
        bool _second_shot;
        std::string _cmd;
};
enum {STANDBY = 0, PI, MANUAL, AUTO};
#define STANDBY_MODE '`'
#define INPUT_WIDTH 200
#define INPUT_HEIGHT 200
