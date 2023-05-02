#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class Camera
{
    public:
        Camera();
        ~Camera();

        cv::Mat capture_frame();
        std::vector<int> get_ids() { return _ids; };
        std::vector<std::vector<cv::Point2f>> get_corners() { return _corners };

    private:

        cv::VideoCapture _vid;
        cv::Mat BGR_frame;

        std::vector<int> _ids;
        std::vector<std::vector<cv::Point2f> > _corners;
        cv::Ptr<cv::aruco::Dictionary> _dictionary;

};
