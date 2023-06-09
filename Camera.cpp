#include "Camera.h"



Camera::Camera() :
    Aruco(frame),
    _Aruco_ON(true)
{
    _vid.open(0, cv::CAP_V4L2);
    if(_vid.isOpened() == false) {
        std::cout << "Camera open failure" << std::endl;
    }
}

Camera::~Camera()
{
    _vid.release();
}


cv::Mat Camera::capture_frame()
{
    _vid >> frame;
    if(frame.empty() == false) {
        if(_Aruco_ON)
        {
            find_markers();
            draw_markers();
        }
        cv::imshow("Image", frame);
        return frame;
    }
    else {
        std::cout << "Frame Capture Failure" << std::endl;
        return cv::Mat::zeros(cv::Size(0,0), CV_8UC3);
    }
}
