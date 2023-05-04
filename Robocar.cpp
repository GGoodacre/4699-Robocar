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
        for(int i; i < test_ids; i++)
        {
            std::cout << "Markers Found: " << test_ids.at(i) << " " << std::endl;
        }
        test_ids.clear();
    }
    switch(_mode)
    {
        case STANDBY:
            if(_key == 't') // ENTER TEST MODE
            {
                _mode = PI;
                whatsgoingon = 0;
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
            _drive.set_direction(180);
            break;
        case 'd':
            _drive.set_direction(90);
            break;
        default:
            _drive.stop();
    }
}

void Robocar::testPI()
{
    std::cout << whatsgoingon << std::endl;
    whatsgoingon++;
    _server.lock();
    _pi_camera = _camera.capture_frame();
    _server.unlock();

    std::vector<int> ids;
    ids = _camera.get_ids();
    if(ids.size() > 0)
    {
        for(int i = 0; i < ids.size(); i++)
        {
            bool match = false;
            for(int j = 0; j < test_ids.size(); j++)
            {
                if(ids.at(i) == test_ids.at(j))
                {
                    match = true;
                }
            }
            if(match == false)
            {
                test_ids.push_back(ids.at(i));
            }
        }
    }

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
