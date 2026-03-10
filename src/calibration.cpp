#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include "calibration.hpp"

void calibration()
{
    std::string images_path = "../../../calibration_images/";

    cv::Size patternSize(4, 3);

    for (int i = 1; i <= 20; i++)   // 20 images
    {
        std::string filename = images_path + "calib" + std::to_string(i) + ".jpg";

        cv::Mat image = cv::imread(filename);

        if (image.empty())
        {
            std::cout << "Fin des images\n";
            break;
        }

        std::vector<cv::Point2f> corners;

        bool found = cv::findChessboardCorners(image, patternSize, corners);

        if (found)
        {
            std::cout << "Damier detecte dans " << filename << "\n";
            cv::drawChessboardCorners(image, patternSize, corners, found);
        }
        else
        {
            std::cout << "Damier non detecte dans " << filename << "\n";
        }

        cv::imshow("Calibration", image);

        int key = cv::waitKey(0);

        if (key == 27)
            break;
    }

    cv::destroyAllWindows();
}