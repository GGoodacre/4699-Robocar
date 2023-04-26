#pragma once
#include <opencv2/opencv.hpp>
#include "CControl.h"
#include <thread>
#include <chrono>


/**
*
* @brief Base class meant to provide the framework for a Canvas that you can commincate with a serial device
*
* This class is only meant to be used as a base class and requires the child class to write its own draw and update method. When run is called it will constantly update and draw
*
* @author Garnett Goodacre
*/
class CBase4618
{
protected:
	CControl _device; ///< Device to communicate with
	cv::Mat _canvas; ///< Canvas to draw on
	bool thread_exit = 0; ///< exits all threads when value goes to true
	char _key = -1;
public:
	/**
	* @brief Starts up the threads and waits for 'q' to quit
	* @return nothing
	*/
	void run();

private:
	/**
	* @brief Pure virtual class for updating anything
	* @return nothing
	*/
	virtual void update() = 0;
	/**
	* @brief Pure virtual class to draw onto the canvas
	* @return nothing
	*/
	virtual void draw() = 0;
};

