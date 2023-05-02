#include "Robo_Server.h"

Robo_Server::Robo_Server()
    _thread_exit(false)
{   
}

Robo_Server::~Robo_Server() 
{
    _thread_exit = true;
    std::this_thread::sleep(std::chrono::milliseconds(10));
}


bool Robo_Server::start(int port, cv::mat& im) 
{
    std::thread th_server_start(port, &Robo_Server::start_server, this);
    std::thread th_server_run(im, &Robo_Server::run_server, this)
    th_start.detach();
    th_server_run.detach();
}

void Robo_Server::start_server(int port, Robo_Server* ptr, std::string addr) 
{
    ptr->this->start(port);
}

void Robo_Server::run_server(cv::mat& im, Robo_Server* ptr) 
{
    cv::Mat frame;
    while (ptr->_thread_exit == false) 
    {
        frame = im;
        ptr->this->set_txim(frame);
        ptr->this->get_cmds(ptr->_cmds);
    }
}

