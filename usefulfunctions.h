#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

double distance(cv::Point2f a, cv::Point2f b);

double angle(cv::Point2f a, cv::Point2f b);

double area_corners(std::vector<cv::Point2f> points);

bool draw_rotated_rect(cv::Mat& im, cv::RotatedRect rect, cv::Scalar colour);
