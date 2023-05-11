#include "Robocar.h"

Robocar::Robocar() :
    _mode(STANDBY),
    _cmd("000000000"),
    _max_power(100),
    _servo_speed(100)
{

    //Initilize OPENCV Window
    cvui::init("Input");
    _input_box = cv::Mat::zeros(cv::Size(INPUT_WIDTH,INPUT_HEIGHT), CV_8UC3);
    _cmd_server.start(PC_PORT, _pi_camera);
    _server.start(IM_PORT, _pi_camera);
    _client.start(ARENA_ADDRESS, ARENA_PORT);

}

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
void Robocar::update()
{
    _drive.set_max_power(_max_power);


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
    cvui::text(_input_box, 20, 20, "POWER: ");
    cvui::trackbar(_input_box, 100, 5, 400,&_max_power, 0, 100);
    cvui::text(_input_box, 20, 70, "SERVO: ");
    cvui::trackbar(_input_box, 100, 55, 400,&_servo_speed, 100, 3000);
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
    cv::Mat test_image;
    std::string test_status;
    _client.lock();
    test_image = _client.get_image();
    _client.unlock();
    test_status = _client.get_status();
    if(test_image.empty() == false)
    {
        cv::imshow("Server", test_image);
    }
    std::cout << "Current Status: " << test_status << std::endl;
    /*
    std::vector<int> ids;
    ids = _camera.get_ids();
    if(ids.size() > 0)
    {
        for(int i = 0; i < ids.size(); i++)
        {
            bool match = false;
            for(int j = 0; j < test_ids.size(); j++)
            {s
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
    */

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
    _server.set_status(_client.get_status());
    _server.unlock();

    _cmd_server.lock();
    std::string cmd;
    cmd = _cmd_server.get_latest_cmd();
    _cmd_server.unlock();

    if (cmd == "EMPTY")
    {
        _cmd = _cmd;
    }
    else
    {
        _cmd = cmd;
    }
    std::cout << "Current CMD: " << _cmd << std::endl;
    telecommunication_drive(_cmd.substr(5,4));
    telecommunication_shoot(_cmd.substr(0,5));

}

void Robocar::telecommunication_drive(std::string cmd)
{
    if (cmd == "0000")
    {
        _drive.stop();
    }
    else if (cmd == "0001")
    {
        _drive.set_direction(0);
    }
    else if (cmd == "0010")
    {
        _drive.set_direction(-90);
    }
    else if (cmd == "0100")
    {
        _drive.set_direction(180);
    }
    else if (cmd == "1000")
    {
        _drive.set_direction(90);
    }
    else if (cmd == "0011")
    {
        _drive.set_direction(-45);
    }
    else if (cmd == "1001")
    {
        _drive.set_direction(45);
    }
    else if (cmd == "0110")
    {
        _drive.set_direction(-135);
    }
    else if (cmd == "1100")
    {
        _drive.set_direction(135);
    }
    else
    {
        _drive.stop();
    }
}

void Robocar::telecommunication_shoot(std::string cmd)
{
    int delta;
    delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - _time_telecom).count();

    int x_change;
    int y_change;
    if (cmd.substr(2,2) == "01")
    {
        y_change = (delta*_servo_speed)/2000;
    }
    else if (cmd.substr(2,2) == "10")
    {
        y_change = -(delta*_servo_speed)/2000;
    }
    else
    {
        y_change = 0;
    }
    if (cmd.substr(0,2) == "01")
    {
        x_change = (delta*_servo_speed)/2000;
    }
    else if (cmd.substr(0,2) == "10")
    {
        x_change = -(delta*_servo_speed)/2000;
    }
    else
    {
        x_change = 0;
    }

    _gun.relative_move(x_change, y_change);

    if(cmd.substr(4) == "1")
    {
        _gun.fire();
    }
}
