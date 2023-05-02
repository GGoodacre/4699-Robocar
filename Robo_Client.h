#pragma once
#include "Client.h"
#include <chrono>

class Robo_Client :
    protected CClient
{
    public:
        Robo_Client();
        ~Robo_Client();

        void start(std::string addr, int port);

        cv::mat get_image();
        std::vector<std::string> get_status();


    private:

        bool _thread_exit;

        static void run_client(Robo_Client* ptr);
        static void image_client(Robo_Client* ptr);

        std::vector<std::string> _status
        cv::mat _im;
};

#define ARENA_PORT 5001