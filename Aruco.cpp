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
    std::cout << "1" << std::endl;
    if(_im.empty() == false) 
    {
        cv::aruco::detectMarkers(_im, _dictionary, _corners, _ids);
        std::cout << "2" << std::endl;
    }
}


void Aruco::draw_markers()
{
    std::cout << "3" << std::endl;
    if(_ids.size() > 0)
    {
        std::cout << "4" << std::endl;
        cv::aruco::drawDetectedMarkers(_im, _corners, _ids);
    }
}
