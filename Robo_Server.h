#pragma once
#include "server.h"
#include <chrono>
#include <vector>
#include <thread>

class Robo_Server :
    protected Server
{
    public:
        Robo_Server();
        ~Robo_Server();

        void start(int port, cv::Mat& im);
        std::vector<std::string> get_cmds() { return _cmds; };
        void lock() {_lock.lock(); };
        void unlock() {_lock.unlock(); };

    protected:
        void start(int port) override { Server::start(port); };
        void stop() override { Server::stop(); };
        void set_txim (cv::Mat &im) override { Server::set_txim(im); };
        void get_cmd (std::vector<std::string> &cmds) override { Server::get_cmd(cmds); };
        void send_string (std::string send_str) override { Server::send_string(send_str); };

    private:

        std::mutex _lock;
        std::vector<std::string> _cmds;


        bool _thread_exit;

        static void start_server(int port, Robo_Server* ptr);
        static void run_server(cv::Mat& im, Robo_Server* ptr);


};

#define PC_PORT 4001
