#include "Robo_Client.h"

Robo_Client::Robo_Client() :
    _thread_exit(false)
{

}

Robo_Client::~Robo_Client()
{
    _thread_exit = true;
    this->close_socket();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void Robo_Client::start(std::string addr, int port)
{
    this->connect_socket(addr, port);

    std::thread th_run(&Robo_Client::run_client, this);
    th_run.detach();
}

std::string Robo_Client::get_status()
{
    return _status;
}

cv::Mat Robo_Client::get_image()
{
    return _im;
}

void Robo_Client::run_client(Robo_Client* ptr)
{
    while (ptr->_thread_exit == false)
    {
        ptr->tx_str("G 0");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ptr->_lock.lock();
        ptr->rx_str(ptr->_status);
        ptr->_lock.unlock();
        ptr->tx_str("G 1");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ptr->_lock.lock();
        ptr->rx_im(ptr->_im);
        ptr->_lock.unlock();
    }
}
