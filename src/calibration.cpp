#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

#include "calibration.hpp"



// fonction qui récupère les images de calibration
void getCalibrationImages(std::string images_path, std::vector<std::string>& images)
{
    int i = 1;

    while (true)
    {
        std::string filename = images_path + "calib" + std::to_string(i) + ".png";

        cv::Mat test = cv::imread(filename);

        // si l'image n'existe pas on arrête
        if (test.empty())
            break;

        images.push_back(filename);

        i++;
    }
}



// fonction principale de calibration
void calibration()
{
    std::string images_path = "../../../calibration_images/";

    // conteneur qui stockera les chemins des images
    std::vector<std::string> images;

    // récupérer les images
    getCalibrationImages(images_path, images);

    // taille du damier (coins internes)
    cv::Size patternSize(7, 9);

    for (const auto& filename : images)
    {
        cv::Mat image = cv::imread(filename);

        if (image.empty())
            continue;

        std::vector<cv::Point2f> corners;

        bool found = cv::findChessboardCorners(image, patternSize, corners);

        if (found)
        {
            std::cout << "Damier detecte dans : " << filename << "\n";

            cv::drawChessboardCorners(image, patternSize, corners, found);
        }
        else
        {
            std::cout << "Damier non detecte dans : " << filename << "\n";
        }

        cv::imshow("Calibration", image);

        // si on ferme la fenêtre
        if (cv::getWindowProperty("Calibration", cv::WND_PROP_VISIBLE) < 1)
            break;

        int key = cv::waitKey(0);

        if (key == 27)
            break;
    }

    cv::destroyAllWindows();
}