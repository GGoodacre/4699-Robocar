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
        for(int i; i < test_ids.size(); i++)
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
            }
            if(_key == 'm') // ENTER TELECOMMUNICATION MODE
            {
                _mode = MANUAL;
                _time_telecom = std::chrono::system_clock::now();
            }
            break;
        case PI:
            drivePI();
            testPI();
            break;
        case MANUAL:
            telecommunication_mode();
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

void Robocar::telecommunication_mode()
{
    _server.lock();
    _pi_camera = _camera.capture_frame();
    _server.unlock();

    std::string cmd;
    cmd = _server.get_latest_cmd();
    std::cout << "Current CMD: " << cmd << std::endl;
    telecommunication_drive(cmd[0:3]);
    telecommunication_shoot(cmd[4:8]);
    
}

void Robocar::telecommunication_drive(std::string cmd)
{
    switch(cmd)
    {
        case "0000":
        {
            _drive.stop();
            break;
        }
        case "0001":
        {
            _drive.set_direction(0);
            break;
        }
        case "0010":
        {
            _drive.set_direction(-90);
            break;
        }
        case "0100":
        {
            _drive.set_direction(180);
            break;
        }
        case "1000":
        {
            _drive.set_direction(90);
            break;
        }
        case "0011":
        {
            _drive.set_direction(-45);
            break;
        }
        case "1001":
        {
            _drive.set_direction(45);
            break;
        }
        case "0110":
        {
            _drive.set_direction(-135);
            break;
        }
        case "1100":
        {
            _drive.set_direction(135);
            break;
        }
        default:
        {
            _drive.stop();
            break;
        }
    }
}

void Robocar::telecommunication_shoot(std::string cmd)
{
    int delta;
    delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time_telecom).count();

    int x_change;
    int y_change;
    switch(cmd[1:2])
    {
        case "01":
        {
            y_change = delta/100;
            break;
        }
        case "10":
        {
            y_change = -delta/100;
            break;
        }
        default:
        {
            y_change = 0;
        }
    }
    switch(cmd[3:4])
    {
        case "01":
        {
            x_change = delta/100;
            break;
        }
        case "10":
        {
            x_change = -delta/100;
            break;
        }
        default:
        {
            y_change = 0;
        }
    }

    _gun.relative_move(x_change, y_change);
    if(cmd[0] == "1" || _second_shot)
    {
        if(_gun.fire())
        {
            _second_shot = false;
        }
        else
        {
            _second_shot = true;
        }
    }
}