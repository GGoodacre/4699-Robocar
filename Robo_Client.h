#pragma once
#include "Client.h"
#include <chrono>
#include <thread>

class Robo_Client :
    protected CClient
{
    public:
        Robo_Client();
        ~Robo_Client();

        void start(std::string addr, int port);

        cv::Mat get_image();
        std::string get_status();

        void lock() {_lock.lock(); };
        void unlock() {_lock.unlock(); };
    private:

        bool _thread_exit;

        static void run_client(Robo_Client* ptr);
        static void image_client(Robo_Client* ptr);

        std::string _status;
        cv::Mat _im;
        cv::Mat _resized;
        std::mutex _lock;
};

#define ARENA_PORT 5001
#define ARENA_ADDRESS "192.168.0.105"
