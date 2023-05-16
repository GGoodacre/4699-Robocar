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
        Robo_Server _cmd_server;
        Robo_Client _client;

        Camera _camera;

        //TESTING
        void drivePI();
        void testPI();


        //TELEOPERATION
        void telecommunication_mode();
        void telecommunication_drive(std::string cmd);
        void telecommunication_shoot(std::string cmd);
        std::chrono::system_clock::time_point _time_telecom;
        bool _second_shot;
        std::string _cmd;

        //AUTOMATIC
        void automatic_mode();
        void automatic_drive(cv::Mat im);
        void automatic_shoot(cv::Mat im);
        int _state;
        int angle_change_x(std::vector<cv::Point2f> corners);
        int angle_change_y(std::vector<cv::Point2f> corners);

        //SETTINGS
        int _max_power;
        int _servo_speed;
};

enum {STANDBY = 0, PI, MANUAL, AUTO};
#define STANDBY_MODE '`'
#define INPUT_WIDTH 600
#define INPUT_HEIGHT 200

//ARUCO MARKERS
#define WEST_MARKER 21
#define NORTH_MARKER 22
#define EAST_MARKER 23
#define SOUTH_MARKER 24
#define CAR_MARKER 32

//Angle calculations
#define MAX_DISTANCE
#define ANGLE_MAXDISTANCE 
#define ANGLE0_DISTANCE

#define COEFF_A1 
#define COEFF_B1
#define COEFF_C1

#define COEFF_A2
#define COEFF_B2
#define COEFF_C2

#define FOCAL_LENGTH 3.04
#define REAL_HEIGHT 
#define SENSOR_HEIGHT 2.76