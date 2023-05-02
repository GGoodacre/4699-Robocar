#include "Robocar.h"

Robocar::Robocar() :
    _mode(STANDBY)
{

    //Initilize OPENCV Window
    cvui::init("Input");
    _input_box = cv::Mat::zeros(cv::Size(INPUT_WIDTH,INPUT_HEIGHT), CV_8UC3);
    _server.start(PC_PORT, _pi_camera);

}

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
void Robocar::update()
{
    if(_key == STANDBY_MODE)
    {
        _mode = STANDBY;
    }
    switch(_mode)
    {
        case STANDBY:
            if(_key == 't') // ENTER TEST MODE
            {
                _mode = PI;
            }
            break;
        case PI:
            drivePI();
            testPI();
            break;
        case MANUAL:
            break;
        case AUTO:
            break;
    }
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
Robocar::~Robocar() {
}


void Robocar::drivePI() {
    switch(_key)
    {
        case 'w':
            _drive.set_direction(0);
            break;
        case 'a':
            _drive.set_direction(-90);
            break;
        case 's':
            _drive.set_direction(90);
            break;
        case 'd':
            _drive.set_direction(180);
            break;
        default:
            _drive.stop();
    }
}

void Robocar::testPI()
{
    _server.lock();
    _pi_camera = _camera.capture_frame();
    _server.unlock();
    std::vector<std::string> cmds;
    cmds = _server.get_cmds();
    if (cmds.size() > 0)
    {
        for (int i = 0; i < cmds.size(); i++)
        {
            std::cout << cmds.at(i) << std::endl;
        }
    }

}
