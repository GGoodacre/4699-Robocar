#include "Camera.h"



Camera::Camera()
{
    _vid.open(0, cv::CAP_V4L2);
    if(_vid.isOpened() == false) {
        std::cout << "Camera open failure" << std::endl;
    }
}


cv::Mat Camera::capture_frame()
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
