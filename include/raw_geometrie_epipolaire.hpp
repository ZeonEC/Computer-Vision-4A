#ifndef RAW_GEOMETRIE_EPIPOLAIRE_HPP
#define RAW_GEOMETRIE_EPIPOLAIRE_HPP

#include <opencv2/opencv.hpp>
#include <string>

void raw_geometrie_epipolaire(
    const cv::Mat& imgLeft,
    const cv::Mat& imgRight,
    const cv::Mat& F
);

#endif