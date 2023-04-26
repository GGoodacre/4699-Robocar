#include "GPIOOutput.h"

class HBridge
{
    public:
        Hbridge(CControl& device, 
                std::mutex& lock,
                bool orientation,
                int port_IN1,
                int port_IN2,
                int port_STBY
                );
        ~Hbridge();

        set_direction(int direction)
    private:
        int _direction;
        bool _orientation;

        GPIOOutput _IN1;
        GPIOOutput _IN2;
        GPIOOutput _STBY;
}