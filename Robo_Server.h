#pragma once
#include "Server.h"
#include <chrono>

class Robo_Server :
    protected Server
{
    public:
        Robo_Server();
        ~Robo_Server();

        bool start(int port, cv::mat& im);
        std::vector<std::string> get_cmds() { return _cmds };

    private:

        std::vector<std::string> _cmds;


        bool _thread_exit;
        //static void server_thread(Communication* ptr);
        static void start_server(int port, Robo_Server* ptr, std::string addr = "");
        static void run_server(cv::mat& im, Robo_Server* ptr);


};

#define PC_PORT 4001