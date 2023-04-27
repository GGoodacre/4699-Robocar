#pragma once
#include "CControl.h"
#include <mutex>

class Component
{
    public:
        void init(
                    int type, int port,
                    bool output = 1,
                    bool pullup = 0);

        void set_value(int value);
        ~Component();
    private:
        int _port;
        int _type;
        CControl _device;
};

enum {CW = 0, CCW};
enum {FORWARD = 0, REVERSE, BRAKE};
