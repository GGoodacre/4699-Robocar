#pragma once
#include <math.h>
#include "pigpio.h"
enum {DIGITAL = 0, ANALOG, SERVO, PWM};

#define MOTOR_LEFT 17
#define MOTOR_RIGHT 27
#define SERVO_X 5
#define SERVO_Y 6
#define SERVO_TRIGGER 12
#define AIN1 3
#define BIN1 10
#define AIN2 4
#define BIN2 22
#define STBY 2

#define OUTPUT 1
#define INPUT 0
#define PULLUP 1

/**
*
* @brief Comunicates with a specified serial port
*
* This class reads and writes data to the specified serial port
*
* @author Garnett Goodacre
*/
class CControl {
private:

public:

	CControl();
	~CControl();

	bool init();

	void gpio_init(int port, bool output = 1, bool pullup = 0);

	/**
	* @brief Reads analog/digital/servo data and sets result to the value
	*
	* @param type Type of read (Digital/Analog/Servo)
	* @param channel Channel number to read
	* @param result Saves the read result to result
	* @return returns true if the read was successful
	*/
	bool get_data(int type, int channel, int& result);

	/**
	* @brief Sets digital/servo channel to given value
	*
	* @param type Type to set (Digital/Servo)
	* @param channel Channel to set
	* @param val Value to set the channel to
	* @return returns true if write was successful
	*/
	bool set_data(int type, int channel, int val);

	/**
	* @brief Gets an analog signal and divdes it by the full value of a 12 bit ADC
	* @param type Should always be set to ANALOG
	* @param channel Channel you wish to get the value as a percentage of
	*/
	float get_analog(int type, int channel);
	/**
	* @brief Gets a debounced digital signal 0 = true, 1 = false
	* @param type Should always be set to ANALOG
	* @param channel Channel you wish to debounce
	* @param timeout How long you wish to disable the input after being pressed
	* @param repeat 0 - Dont repeat if you hold assertion 1 - repeat as the input is asserted
	*/
	bool get_button(int  type, int channel, int timeout = 1000, int repeat = 0);
};

#define REPEAT 1
#define SINGLE 0
