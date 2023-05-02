#pragma once
#include <opencv2/opencv.hpp>

class Camera
{
    public:
        Camera();

        cv::Mat capture_frame();

    private:

        cv::VideoCapture _vid;
        cv::Mat BGR_frame;

};
