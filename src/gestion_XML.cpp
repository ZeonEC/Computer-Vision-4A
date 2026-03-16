#include <iostream>
#include <opencv2/opencv.hpp>

#include "gestion_XML.hpp"


// charge les paramètres de calibration depuis le fichier XML
void gestion_XML()
{
    // chemin du fichier
    std::string filename =
        "../../../calibration_images/results/calibration_cam1_result.xml";

    // ouverture du fichier XML
    cv::FileStorage fs(filename, cv::FileStorage::READ);

    if (!fs.isOpened())
    {
        std::cout << "Impossible d'ouvrir le XML\n";
        return;
    }

    // paramètres de calibration
    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;

    // lecture des données
    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;

    fs.release();

    std::cout << "Calibration chargee : " << filename << std::endl;

    // affichage simple
    std::cout << "\nCamera matrix :\n" << camera_matrix << std::endl;
    std::cout << "\nDistortion coefficients :\n" << dist_coeffs << std::endl;
}