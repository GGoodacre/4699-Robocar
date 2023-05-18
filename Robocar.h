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
        cv::Mat _empty_im;
        cv::Mat _input_box;
        cv::Mat _snapshot;
        cv::Mat _client_image;

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
        bool automatic_drive(cv::Mat im);
        void automatic_shoot(cv::Mat im);
        int _state;
        bool _in_location;

        double angle_change_x(std::vector<cv::Point2f> corners);
        double angle_change_y(std::vector<cv::Point2f> corners);

        void state_change();
        //SETTINGS
        int _max_power;
        int _servo_speed;
};

enum {STANDBY = 0, PI, MANUAL, AUTO};
#define STANDBY_MODE '`'
#define INPUT_WIDTH 600
#define INPUT_HEIGHT 200

//ARUCO MARKERS
//#define WEST_MARKER 21
//#define NORTH_MARKER 22
//#define EAST_MARKER 27
//#define SOUTH_MARKER 23
#define CAR_MARKER 26

#define WEST_MARKER -1
#define NORTH_MARKER -1
#define EAST_MARKER -1
#define SOUTH_MARKER -1

//Angle calculations
#define MAX_DISTANCE 0.782336
#define ANGLE_MAXDISTANCE 43
#define ANGLE0_DISTANCE 0.270914

#define COEFF_A1 65.419
#define COEFF_B1 2.9301
#define COEFF_C1 -3.8349

#define COEFF_A2 -49.698
#define COEFF_B2 -11.899
#define COEFF_C2 87.777

#define FOCAL_LENGTH 3.04
#define REAL_HEIGHT 25.4
#define SENSOR_HEIGHT 2.76
