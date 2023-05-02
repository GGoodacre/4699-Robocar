#include "Camera.h"

cv::mat Camera::capture_frame()
{
    _vid >> BGR_frame;
    if(BGR_frame.empty() == false) {
        cv::imshow("Image", BGR_frame);
        return BGR_frame;
    }
    else {
        std::cout << "Frame Capture Failure" << std::endl;
        return cv::Mat::zeros(cv::Size(0,0), CV_8UC3);
    }
}