#include "Aruco.h"


Aruco::Aruco(cv::Mat &im) :
    _dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250)),
    _im(im)
{
}

void Aruco::find_markers()
{
    if(_im.empty() == false)
    {
        cv::aruco::detectMarkers(_im, _dictionary, _corners, _ids);
    }
}


void Aruco::draw_markers()
{
    if(_ids.size() > 0)
    {
        cv::aruco::drawDetectedMarkers(_im, _corners, _ids);
    }
}
