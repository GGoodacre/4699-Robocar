#include "Robocar.h"
#include "usefulfunctions.h"
#include <regex>

Robocar::Robocar() :
    _mode(STANDBY),
    _cmd("000000000"),
    _max_power(100),
    _servo_speed(5),
    _empty_im(cv::Mat::zeros(cv::Size(10,10), CV_8UC3)),
    _snapshot(cv::Mat::zeros(cv::Size(640,480), CV_8UC3)),
    _client_image(cv::Mat::zeros(cv::Size(600,600), CV_8UC3)),
    _pi_camera(cv::Mat::zeros(cv::Size(640,480), CV_8UC3))
{

    //Initilize OPENCV Window
    cvui::init("Input");
    _input_box = cv::Mat::zeros(cv::Size(INPUT_WIDTH,INPUT_HEIGHT), CV_8UC3);
    _client.start(ARENA_ADDRESS, ARENA_PORT);
    _cmd_server.start_cmd(PC_PORT);
    _server.start(IM_PORT, _snapshot);

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
            else if(_key == 'm') // ENTER TELECOMMUNICATION MODE
            {
                _mode = MANUAL;
                _time_telecom = std::chrono::system_clock::now();
                _state = 0;
            }
            else if(_key == 'a')
            {
                _mode = AUTO;
                _state = 4;
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
            automatic_mode();
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
    cvui::trackbar(_input_box, 100, 55, 400,&_servo_speed, 0, 20);
    cv::imshow("Input", _input_box);
    return;
}

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
Robocar::~Robocar() {
    _drive.stop();
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
    test_status = _client.get_status();
    _client.unlock();

    if(test_image.empty() == false)
    {
        cv::imshow("Server", test_image);
        cv::imwrite("arena.jpg", test_image);
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

    std::vector<std::string> cmds;
    cmds = _server.get_cmds();
    if (cmds.size() > 0)
    {
        for (int i = 0; i < cmds.size(); i++)
        {
            std::cout << cmds.at(i) << std::endl;
        }
    }
    */
}

void Robocar::telecommunication_mode()
{

    _client.lock();
    _server.lock();

    std::string cmd;
    cmd = _cmd_server.get_latest_cmd();
    _snapshot = _client.get_image();
    _server.set_status(std::to_string(_state));

    _server.unlock();
    _client.unlock();

    _in_location = false;


    if (cmd == "EMPTY")
    {
        _cmd = _cmd;
    }
    else
    {
        _cmd = cmd;
    }
    std::cout << "Current CMD: " << _cmd << std::endl;

    state_change();
    telecommunication_drive(_cmd.substr(5,4));
    telecommunication_shoot(_cmd.substr(0,5));
    automatic_shoot(_snapshot);

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

void Robocar::automatic_mode()
{
    //std::cout << "AM" << std::endl;
    _server.lock();
    _client.lock();

    _client_image = _client.get_image();
    _pi_camera = _camera.capture_frame();
    _snapshot = _pi_camera;

    _client.unlock();
    _server.unlock();
    //client_image = cv::imread("arena.jpg");
    cv::Mat shoot_image;
    _client_image.copyTo(shoot_image);


    state_change();
    if(_client_image.empty() == false)
    {
        _in_location = automatic_drive(_client_image);
    }
    automatic_shoot(shoot_image);
}

bool Robocar::automatic_drive(cv::Mat im)
{
    //std::cout << "AD" << std::endl;

    cv::Point2f desired_location;
    Aruco aruco = Aruco(im);
    aruco.find_markers();

    aruco.draw_markers();
    //cv::imshow("Top Down", im);

    std::vector<int> ids = aruco.get_ids();


    int car = -1;
    if(ids.size() > 0)
    {
        for(int i = 0; i < ids.size(); i++) {
            if(ids.at(i) == CAR_MARKER)
            {
                car = i;
                break;
            }
        }
    }
    if(car != -1)
    {
        std::vector<cv::Point2f> car_location = aruco.get_corners().at(car);
        cv::Point2f car_center = cv::Point2f((car_location.at(0).x + car_location.at(2).x)/2,(car_location.at(0).y + car_location.at(2).y)/2);
        double car_angle = atan2((car_location.at(1).y - car_location.at(0).y),(car_location.at(0).x-car_location.at(1).x)) * (180/M_PI);
        //for(int i = 0; i < 4; i++)
        //{
        //    std::cout << "ID: " << i << " X: " << car_location.at(i).x << " Y: " << car_location.at(i).y << std::endl;
        //}
        //std::cout << "Car Center x: " << car_center.x << " y: " << car_center.y << std::endl;
        //std::cout << "Car Angle: " << car_angle << std::endl;
        switch(_state)
        {
            case(2):
            {
                desired_location = cv::Point2f(70, 395);
                break;
            }
            case(3):
            {
                desired_location = cv::Point2f(257, 356);
                break;
            }
            case(4):
            {
                desired_location = cv::Point2f(517, 560);
                break;
            }
            default:
            {
                desired_location = car_center;
                break;
            }
        }
        //std::cout << "Next Location x: " << desired_location.x << " y: " << desired_location.y << std::endl;

        if(distance(desired_location, car_center) < 30)
        {
            //std::cout << "TOO CLOSE" << std::endl;
            _drive.stop();
            return true;
        }
        else
        {
            bool loop_exit = false;
            int i = 0;
            int cw = 0;
            double d = distance(car_center, desired_location);
            double a;
            cv::RotatedRect car_path;

            do
            {
                a = angle(car_center, desired_location);
                cv::Point2f middle_point = cv::Point2f((car_center.x+desired_location.x)/2,(car_center.y+desired_location.y)/2);
                car_path = cv::RotatedRect(middle_point, cv::Size2f(40,distance(desired_location, car_center)), angle(cv::Point2f(car_center.y,car_center.x), cv::Point2f(desired_location.y,desired_location.x)));

                switch(barrier_hit(car_path))
                {
                    case 1:
                    case 2:
                    {
                        if(cw == 0)
                        {
                            cw = 1;
                            i = i + 1;
                        }
                        else if(cw == 1)
                        {
                            i = i + 1;
                        }
                        else if(cw == 2)
                        {
                            i = i - 1;
                        }

                        desired_location = cv::Point2f(d*cos((a+i)*(M_PI/180)) + car_center.x,-d*sin((a+i)*(M_PI/180)) + car_center.y);
                        break;
                    }
                    case 3:
                    {
                        if(cw == 0)
                        {
                            cw = 2;
                            i = i - 1;
                        }
                        else if(cw == 1)
                        {
                            i = i + 1;
                        }
                        else if(cw == 2)
                        {
                            i = i - 1;
                        }
                        desired_location = cv::Point2f(d*cos((a+i)*(M_PI/180)) + car_center.x, -d*sin((a+i)*(M_PI/180)) + car_center.y);
                        break;
                    }
                    case 0:
                    {
                        if(cw != 0)
                        {
                            d = 100;
                        }
                        _drive.go_until(car_angle - a, d*0.002032);
                        loop_exit = true;
                        break;
                    }
                }
                if(i >= 360 || i <= -360)
                {
                    loop_exit = true;
                    std::cout << "IM STUCK" << std::endl;
                }
            } while(loop_exit == false);

            //////////////////////////////// TESTING CODE
            draw_rotated_rect(im, car_path, cv::Scalar(255,255,255));
            cv::circle(im, desired_location, 4, cv::Scalar(0,255,0));
            cv::circle(im, car_center, 4, cv::Scalar(0,0,255));
            cv::imshow("Rects", im);
            std::cout << "Car Angle: " << car_angle << std::endl;
            std::cout << "Angle: " << a << std::endl;
            std::cout << "Direction: " << car_angle - a << std::endl;
            std::cout << "Distance to target: " << distance(desired_location, car_center)*0.002032 << std::endl;
            ////////////////////////////////

            return false;
        }
    }
}

void Robocar::automatic_shoot(cv::Mat im)
{
    //std::cout << "AS" << std::endl;
    int desired_marker;
    double angle_x;
    double angle_y;

    switch(_state)
    {
        case(0):
        {
            desired_marker = NORTH_MARKER;
            break;
        }
        case(1):
        {
            desired_marker = EAST_MARKER;
            break;
        }
        case(2):
        {
            desired_marker = WEST_MARKER;
            break;
        }
        case(3):
        {
            desired_marker = SOUTH_MARKER;
            break;
        }
        default:
        {
            desired_marker = -1;
            break;
        }
    }

    std::vector<int> ids = _camera.get_ids();
    int target_found = -1;

    if (ids.size() > 0)
    {
        for(int i = 0; i < ids.size(); i++) {
            if(ids.at(i) == desired_marker)
            {
                target_found = i;
                break;
            }
        }
    }

    if(target_found == -1)
    {
        if(im.empty() == false)
        {
            Aruco aruco = Aruco(im);
            aruco.find_markers();

            std::vector<int> ids = aruco.get_ids();

            int car = -1;
            if(ids.size() > 0)
            {
                for(int i = 0; i < ids.size(); i++) {
                    if(ids.at(i) == CAR_MARKER)
                    {
                        car = i;
                        break;
                    }
                }
            }
            if(car != -1)
            {
                std::vector<cv::Point2f> car_location = aruco.get_corners().at(car);
                cv::Point2f car_center = cv::Point2f((car_location.at(0).x + car_location.at(1).x)/2,(car_location.at(0).y + car_location.at(2).y)/2);
                double car_angle = atan2((car_location.at(1).y - car_location.at(0).y),(car_location.at(0).x-car_location.at(1).x)) * (180/M_PI);
                double d;
                double marker_angle;
                switch(_state)
                {
                    case(0):
                    {
                        marker_angle = atan2(car_center.y,(im.size().width/2+50 - car_center.x));
                        marker_angle = marker_angle * (180/M_PI);
                        d = distance(car_center, cv::Point2f(im.size().width/2,0));
                        break;
                    }
                    case(1):
                    {
                        marker_angle = atan2((car_center.y - im.size().height/2),(im.size().width - car_center.x));
                        marker_angle = marker_angle * (180/M_PI);
                        d = distance(car_center, cv::Point2f(im.size().width,im.size().height/2));
                        break;
                    }
                    case(2):
                    {
                        marker_angle = atan2((car_center.y - im.size().height/2),(-car_center.x));
                        marker_angle = marker_angle * (180/M_PI);
                        d = distance(car_center, cv::Point2f(0,im.size().height/2));
                        break;
                    }
                    case(3):
                    {
                        marker_angle = atan2((car_center.y - im.size().height),(im.size().width/2 - car_center.x));
                        marker_angle = marker_angle * (180/M_PI);
                        d = distance(car_center, cv::Point2f(im.size().width/2,im.size().height));
                        break;
                    }
                    default:
                    {
                        marker_angle = car_angle;
                        break;
                    }
                }
                angle_x = (car_angle - marker_angle);
                if(angle_x < -180)
                {
                    angle_x = 360 + angle_x;
                }
                if(angle_x > 180)
                {
                    angle_x = angle_x - 360;
                }
                //std::cout << "Marker Angle: " << marker_angle << std::endl;
                //std::cout << "Car Angle: " << car_angle << std::endl;
                //std::cout << "New Angle: " << angle_x << std::endl;

                d = d*0.002032;
                if(d < ANGLE0_DISTANCE)
                {
                    angle_y = 0;
                }
                else if(d > MAX_DISTANCE)
                {
                    angle_y = ANGLE_MAXDISTANCE;
                }
                else
                {
                    angle_y = COEFF_A1*pow(d,2) + COEFF_B1*d + COEFF_C1;
                }
                //std::cout << "Distance: " << d << std::endl;
                //std::cout << "Y ANGLE: " << angle_y << std::endl;
                _gun.absolute_move(angle_x*-12.5 + 1500, angle_y*5.556 + 1000);
                if((angle_x < 70) && (angle_x > -70))
                {
                    //std::cout << "IM TRYING TO FIRE" << std::endl;
                    if(_state != 4)
                    {
                        _gun.fire();
                    }
                }
                else
                {
                    if(_in_location)
                    {
                        _drive.go_until(angle_x, d);
                    }
                }
            }
        }
    }
    else
    {
        std::vector<std::vector<cv::Point2f>> corners = _camera.get_corners();
        angle_x = angle_change_x(corners.at(target_found));
        angle_y = angle_change_y(corners.at(target_found));
        //std::cout << "Angle y: " << angle_y << std::endl;
        _gun.auto_move(angle_x, angle_y);
    }

}

double Robocar::angle_change_x(std::vector<cv::Point2f> corners)
{
    double x = (corners.at(0).x + corners.at(1).x + corners.at(2).x + corners.at(3).x)/4;
    double y = (corners.at(0).y + corners.at(1).y + corners.at(2).y + corners.at(3).y)/4;
    /*
    if(x == 0)
    {
        return 0;
    }
    else
    {
        /ouble angle_radians = atan2((_pi_camera.size().height - y),(x - _pi_camera.size().width / 2));
        //std::cout << "x: " << _pi_camera.size().width << " y: " << _pi_camera.size().height << std::endl;
        //std::cout << "x: " << x << " y: " << y << " angle: " << angle_radians*(180/M_PI) << std::endl;
        return -(90 - angle_radians*(180/M_PI));
    }
    */
    double angle = -(x*0.015625-5);
    if(x < 400 && x > 200)
    {
        _gun.fire();
    }
    return angle;
}

double Robocar::angle_change_y(std::vector<cv::Point2f> corners)
{
    //double h = abs(corners.at(0).y * corners.at(1).y - corners.at(2).y * corners.at(3).y)/2;
    double a = pow(area_corners(corners),0.5);
    double d = (FOCAL_LENGTH*REAL_HEIGHT*_snapshot.size().height)/(a*SENSOR_HEIGHT)/1000;
    //std::cout << "Distance: " << std::to_string(d) << std::endl;
    if(d < ANGLE0_DISTANCE)
    {
        //return COEFF_A2*pow(d,2) + COEFF_B2*d + COEFF_C2;
        return 0;
    }
    else if(d > MAX_DISTANCE)
    {
        return ANGLE_MAXDISTANCE;
    }
    else
    {
        return COEFF_A1*pow(d,2) + COEFF_B1*d + COEFF_C1;
    }

}

void Robocar::state_change()
{
    _client.lock();
    std::string status = _client.get_status();
    _client.unlock();
    //status = "<ARENA TIME=\"0.000000\" D1=\"0\" D2=\"0\" D3=\"0\" D4=\"0\" />";
    //std::cout << status << std::endl;

    if(std::regex_search(status, std::regex("^<(.*/>)")))
    {
        switch(_state)
        {
            case 0:
            {
                if(std::regex_search(status, std::regex("D2=\"[1-9]\"")))
                {
                    _state++;
                    std::cout << "Current State: " << _state << std::endl;
                }
                break;
            }
            case 1:
            {
                if(std::regex_search(status, std::regex("D3=\"[1-9]\"")))
                {
                    _state++;
                    std::cout << "Current State: " << _state << std::endl;
                }
                break;
            }
            case 2:
            {
                if(std::regex_search(status, std::regex("D1=\"[1-9]\"")))
                {
                    _state++;
                    std::cout << "Current State: " << _state << std::endl;
                }
                break;
            }
            case 3:
            {
                if(std::regex_search(status, std::regex("D4=\"[1-9]\"")))
                {
                    _state++;
                    std::cout << "Current State: " << _state << std::endl;
                }
                break;
            }
        }
    }
    //std::cout << "Current State: " << _state << std::endl;
}
