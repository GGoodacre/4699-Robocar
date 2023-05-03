#pragma once
#include "stdafx.h"

class Aruco 
{
    public:
        Aruco();
        Aruco(cv::Mat& im);

        std::vector<int> get_ids() { return _ids; };
        std::vector<std::vector<cv::Point2f>> get_corners() { return _corners };

    protected:
        std::vector<int> _ids;
        std::vector<std::vector<cv::Point2f> > _corners;
        cv::Ptr<cv::aruco::Dictionary> _dictionary;
    
    private:
        cv::Mat _im;

};