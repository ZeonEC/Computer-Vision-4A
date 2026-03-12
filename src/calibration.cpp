#include <iostream>
#include <opencv2/opencv.hpp>

#include "calibration.hpp"

std::vector<std::vector<cv::Point2f>> get_grid_points(int nb_calib)
{
    std::vector<std::vector<cv::Point2f>> images_grid_points;

    cv::Size pattern_size(7, 9);

    for (int i = 1; i <= nb_calib; i++)
    {
        std::string filename =
            "../../../calibration_images/calib" +
            std::to_string(i) + ".png";

        cv::Mat image = cv::imread(filename);

        if (image.empty())
            continue;

        std::vector<cv::Point2f> corners;

        bool found =
            cv::findChessboardCorners(image, pattern_size, corners);

        if (found)
        {
            images_grid_points.push_back(corners);

            cv::drawChessboardCorners(image, pattern_size, corners, found);
            cv::imshow("Corners", image);
            cv::waitKey(500);
        }
    }

    cv::destroyAllWindows();

    return images_grid_points;
}


void show_grid_points(
    std::vector<std::vector<cv::Point2f>>& images_grid_points)
{
    int i = 1;

    for (const auto& corners : images_grid_points)
    {
        std::cout << "Image " << i << std::endl;

        for (const auto& p : corners)
            std::cout << p << std::endl;

        i++;
    }
}


void calibration(
    std::vector<std::vector<cv::Point2f>>& calibration_points,
    cv::Size& taille_image)
{
    std::cout << "Calibration lancee..." << std::endl;

    std::cout << "Nombre d'images utilisees : "
        << calibration_points.size()
        << std::endl;
}