#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "Aruco.h"

class Camera :
    public Aruco
{
    public:
        Camera();
        ~Camera();

        cv::Mat capture_frame();
        void enable_Aruco() { _Aruco_ON = true; }
        void disable_Aruco() { _Aruco_ON = false; }
    private:

        cv::VideoCapture _vid;
        cv::Mat frame;

        bool _Aruco_ON;


};
