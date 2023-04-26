#include "CControl.h"
#include "pigpio.h"
#include <iostream>
#include <chrono>

CControl::CControl()
{
	init();
}

CControl::~CControl()
{
	gpioTerminate();
}

bool CControl::init()
{
	if(gpioInitialise() < 0) {
		return 0;
	}
	else {
		return 1;
	}
}

void CControl::gpio_init(int port, bool output, bool pullup)
{
	if(output)
	{
		gpioSetMode(port, PI_OUTPUT);
	}
	else
	{
		gpioSetMode(port, PI_INPUT);
	}
	if(pullup)
	{
		gpioSetMode(port, PI_PUD_UP);
	}
}

bool CControl::get_data(int type, int channel, int& result)
{
	if(type == DIGITAL || type == SERVO) {
		result = gpioRead(channel);
		return true;
	}
	if(type == ANALOG) {
        int read_val = 0;
		unsigned char inBuf[3];
		char cmd[] = {1, (8 + channel) << 4, 0};
		int handle = spiOpen(0, 2000000, 0);

		spiXfer(handle, cmd, (char*) inBuf, 3);
		read_val = ((inBuf[1] & 3) << 8) | inBuf[2];

        result = read_val;
		spiClose(handle);
		return true;
	}
	else {
		return false;
	}
}

bool CControl::set_data(int type, int channel, int val)
{
	if(type == DIGITAL) 
	{
		gpioWrite(channel, val);
		return true;
	}
	if(type == SERVO) 
	{
		gpioServo(channel, val);
		return true;
	}
	if(type == PWM) 
	{
		gpioPWM(channel, val)
		return true;
	}
	else 
	{
		return false;
	}
}

float CControl::get_analog(int type, int channel) {

	int result;
	if (get_data(type, channel, result)) {
		float float_result = round(((result / 1024.0) * 100)) / 100;
		return float_result;
	}
	else {
		std::cout << "GET DATA TIMEOUT" << std::endl;
		return 0;
	}

}

bool CControl::get_button(int type, int channel, int timeout, int repeat) {
;
	static std::chrono::system_clock::time_point time_chrono[41] = { std::chrono::system_clock::now() };
	static int button_disable[41] = { 0 };
	auto calc_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - time_chrono[channel]);
	int result = 1;
	if (channel > 40) {
		return false;
	}
	if (button_disable[channel] == 0) {
		get_data(type, channel, result);
		if (!result) {
            time_chrono[channel] = std::chrono::system_clock::now();
			button_disable[channel] = 1;
			return true;
		}
	}
	else if (button_disable[channel] == -1) {
		get_data(type, channel, result);
		if (result) {
			button_disable[channel] = 0;
		}
	}
	else if (calc_elapsed.count() > timeout)
	{
		button_disable[channel] = repeat - 1;
	}
	return false;
}

