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
    double delta_y = a.y - b.y;
    double delta_x = b.x - a.x;
    //std::cout << "Y: " << delta_y << " X: " << delta_x << std::endl;
    angle = atan2(delta_y, delta_x) * (180 / M_PI);
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


bool draw_rotated_rect(cv::Mat& im, cv::RotatedRect rect, cv::Scalar colour)
{
    if(im.empty() == false)
    {
        cv::Point2f vertices2f[4];
        rect.points(vertices2f);

        cv::Point vertices[4];
        for(int i = 0; i < 4; ++i){
            vertices[i] = vertices2f[i];
        }

        // Now we can fill the rotated rectangle with our specified color
        cv::fillConvexPoly(im,
                        vertices,
                        4,
                        colour);
        return true;
    }
    else
    {
        return false;
    }
}

int barrier_hit(cv::RotatedRect rect)
{
    cv::Mat output;
    cv::RotatedRect Barrier_1 = cv::RotatedRect(cv::Point2f(109,1000),cv::Point2f(109,429),cv::Point2f(134,429));
    cv::RotatedRect Barrier_2 = cv::RotatedRect(cv::Point2f(469,1000),cv::Point2f(469,429),cv::Point2f(510,429));
    cv::RotatedRect Barrier_3 = cv::RotatedRect(cv::Point2f(260,296),cv::Point2f(260,224),cv::Point2f(337,224));

    if (cv::rotatedRectangleIntersection(rect,Barrier_1,output) != cv::INTERSECT_NONE)
    {
        return 1;
    }
    else if (cv::rotatedRectangleIntersection(rect,Barrier_2,output) != cv::INTERSECT_NONE)
    {
        return 2;
    }
    else if (cv::rotatedRectangleIntersection(rect,Barrier_3,output) != cv::INTERSECT_NONE)
    {
        return 3;
    }
    else
    {
        return 0;
    }

}
