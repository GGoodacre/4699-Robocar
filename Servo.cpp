#include "Servo.h"

Servo::Servo(int port)
{
    init(SERVO, port);
}