#include "Robocar.h"

Robocar::Robocar()
{
    //Initialize Left Motor
    _motor_left = Motor(    &_device,
                            &_gpio_lock,
                            CCW,
                            AIN1,
                            AIN2,
                            STBY,
                            MOTOR_LEFT,
                            );
    //Initialize Right Motor
    _motor_right = Motor(   &_device,
                            &_gpio_lock,
                            CW,
                            BIN1,
                            BIN2,
                            STBY,
                            MOTOR_RIGHT,
                            );

    //Initilize OPENCV Window
    cvui::init("Input");
    _input_box = cv::Mat::zeros(cv::Size(INPUT_WIDTH,INPUT_HEIGHT), CV_8UC3);

}

////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////
void Robocar::update() 
{
    move();
    return;
}

////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////
void Robocar::draw() 
{
    _input_box = cv::Mat::zeros(cv::Size(INPUT_WIDTH,INPUT_HEIGHT), CV_8UC3);
    cv::imshow("Input", _input_box);
    return;
}

////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////
void Robocar::move() 
{
    switch(_key) 
    {
        case 'w':
            _motor_left.set_direction(FORWARD);
            _motor_left.set_power(20);
            _motor_right.set_direction(FORWARD);
            _motor_right.set_power(20);
            break;
        case 'a':
            _motor_left.set_direction(REVERSE);
            _motor_left.set_power(20);
            _motor_right.set_direction(FORWARD);
            _motor_right.set_power(20);
            break;
        case 's':
            _motor_left.set_direction(REVERSE);
            _motor_left.set_power(20);
            _motor_right.set_direction(REVERSE);
            _motor_right.set_power(20);
            break;
        case 'd':
            _motor_left.set_direction(FORWARD);
            _motor_left.set_power(20);
            _motor_right.set_direction(REVERSE);
            _motor_right.set_power(20);
            break;
        case default:
            _motor_left.set_direction(BRAKE);
            _motor_right.set_direction(BRAKE);
            break;
    }
}


////////////////////////////////////////////////////////////////////////////
// PARAMETERS
////////////////////////////////////////////////////////////////////////////
#define INPUT_WIDTH 600
#define INPUT_HEIGHT 700