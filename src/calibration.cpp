#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include "calibration.hpp"

void calibration()
{
    std::string images_path = "../../../calibration_images/";
    cv::Size patternSize(9, 6);

    for (const auto& entry : std::filesystem::directory_iterator(images_path))
    {
        std::string filename = entry.path().string();

        std::cout << "Lecture : " << filename << std::endl;

        cv::Mat image = cv::imread(filename);

        if (image.empty())
        {
            std::cout << "Impossible de charger l'image\n";
            continue;
        }

        std::vector<cv::Point2f> corners;

        bool found = cv::findChessboardCorners(image, patternSize, corners);

        if (found)
        {
            std::cout << "Damier detecte\n";
            cv::drawChessboardCorners(image, patternSize, corners, found);
        }
        else
        {
            std::cout << "Damier non detecte\n";
        }

        cv::imshow("Calibration", image);

        std::cout << "Appuyer sur SPACE pour l'image suivante (ECHAP pour quitter)\n";

        int key = cv::waitKey(0);

        if (key == 27)
            break;
    }

    cv::destroyAllWindows();
}