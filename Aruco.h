#pragma once
#include "stdafx.h"

class Aruco
{
    public:
        Aruco(cv::Mat& im);

        std::vector<int> get_ids() { return _ids; };
        std::vector<std::vector<cv::Point2f>> get_corners() { return _corners; };
        void set_ArucoImage(cv::Mat &im) { _im = im; };
        void find_markers();
        void draw_markers();

    protected:
        std::vector<int> _ids;
        std::vector<std::vector<cv::Point2f> > _corners;
        cv::Ptr<cv::aruco::Dictionary> _dictionary;



    private:
        cv::Mat& _im;

};
