#include "Robo_Server.h"

Robo_Server::Robo_Server():
    _thread_exit(false)
{
}

Robo_Server::~Robo_Server()
{
    _thread_exit = true;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


void Robo_Server::start(int port, cv::Mat& im)
{

    std::thread th_server_start(&Robo_Server::start_server, port, this);
    std::thread th_server_run(&Robo_Server::run_server, std::ref(im), this);
    th_server_start.detach();
    th_server_run.detach();
}

std::vector<std::string> Robo_Server::get_cmds()
{
    get_cmd(_cmds);
    return _cmds;
}

std::string Robo_Server::get_latest_cmd()
{
    get_cmds();
    if (_cmds.size() > 0)
    {
        return _cmds[_cmds.size()-1];
    }
    else
    {
        return "EMPTY";
    }
}

void Robo_Server::start_server(int port, Robo_Server* ptr)
{
    ptr->start(port);
}

void Robo_Server::run_server(cv::Mat& im, Robo_Server* ptr)
{
    cv::Mat frame;
    std::vector<std::string> cmds;
    while (ptr->_thread_exit == false)
    {
        ptr->_lock.lock();
        frame = im;
        if(frame.empty() == false)
        {
            cv::cvtColor(frame,frame,cv::COLOR_RGB2GRAY);
            cvui::text(frame, 20, 20, ptr->_status);
        }
        ptr->_lock.unlock();
        ptr->set_txim(frame);
        /*
        ptr->get_cmd(cmds);
        ptr->_cmds = cmds;
        */
    }
}

