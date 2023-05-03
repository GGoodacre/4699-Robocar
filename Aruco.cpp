#include "Aruco.h"

Aruco::Aruco() :
    _dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250))
{
}

Aruco::Aruco(cv::Mat &im) :
    _dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250)),
    _im(im)
{
}

void Aruco::find_markers()
{
    cv::aruco::detectMarkers(_im, _dictionary, _corners, _ids);
}


void Aruco::draw_markers()
{
    if(_ids.size() > 0)
    {
        cv::aruco::drawDetectedMarkers(_im, _corners, _ids);
    }
}
