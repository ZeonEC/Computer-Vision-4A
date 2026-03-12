#include <iostream>
#include <opencv2/opencv.hpp>

#include "charger_calibration.hpp"

void charger_calibration()
{
    std::string filename =
        "../../../calibration_images/results/calibration_cam1_result.xml";

    cv::FileStorage fs(filename, cv::FileStorage::READ);

    if (!fs.isOpened())
    {
        std::cout << "Impossible d'ouvrir le XML\n";
        return;
    }

    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;

    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;

    fs.release();

    std::cout << "Calibration chargee depuis : " << filename << std::endl;
}