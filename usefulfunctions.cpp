#include "usefulfunctions.h"
#include <math.h>

double distance(cv::Point2f a, cv::Point2f b)
{
    double dist;
    dist = pow(pow((a.x-b.x),2)+pow((a.y-b.y),2),0.5);
    return dist;
}

double angle(cv::Point2f a, cv::Point2f b)
{
    double angle;
    angle = atan2(b.y - a.y, b.x - a.x) * (180 / M_PI);
    return angle;
}

double area_corners(std::vector<cv::Point2f> points)
{
    if(points.size() != 4)
    {
        return 0;
    }
    else
    {
        double a = points.at(0).x*points.at(1).y - points.at(0).y*points.at(1).x;
        double b = points.at(1).x*points.at(2).y - points.at(1).y*points.at(2).x;
        double c = points.at(2).x*points.at(3).y - points.at(2).y*points.at(3).x;
        double d = points.at(3).x*points.at(0).y - points.at(3).y*points.at(0).x;
        return abs((a+b+c+d)/2);
    }
}
