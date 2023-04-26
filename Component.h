#pragma once
#include "CControl.h"
#include <thread>

class Component
{ 
    public:
        void init(  CControl& device, 
                    std::mutex& lock, 
                    int type, int port, 
                    bool output = 1, 
                    bool pullup = 0);
        
        void set(int value);
    private:
        int _port;
        int _type;
        CControl* _device;
        std::mutex* _lock;
};

enum {CW = 0, CCW};
enum {FORWARD = 0, REVERSE, BRAKE};