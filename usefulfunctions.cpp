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
