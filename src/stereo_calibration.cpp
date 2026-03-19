#include "stereo_calibration.hpp"
#include "gestion_XML.hpp"

#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

void stereo_calibration()
{
    cv::Mat camera_matrix1, dist_coeffs1;
    cv::Mat camera_matrix2, dist_coeffs2;

    std::vector<cv::Mat> rvecs1, tvecs1;
    std::vector<cv::Mat> rvecs2, tvecs2;

    std::vector<double> perViewErrors1, perViewErrors2;

    double RMS1 = 0.0;
    double RMS2 = 0.0;

    cv::Size taille_image1, taille_image2;
    cv::Size pattern_size1, pattern_size2;

    float square_size1 = 0.0f;
    float square_size2 = 0.0f;

    std::vector<std::vector<cv::Point2f>> image_points1, image_points2;

    get_calibration_from_xml(
        "../../../calibration_images/results/calibration_cam0_result.xml",
        camera_matrix1,
        dist_coeffs1,
        rvecs1,
        tvecs1,
        perViewErrors1,
        RMS1,
        taille_image1,
        pattern_size1,
        square_size1,
        image_points1
    );

    get_calibration_from_xml(
        "../../../calibration_images/results/calibration_cam1_result.xml",
        camera_matrix2,
        dist_coeffs2,
        rvecs2,
        tvecs2,
        perViewErrors2,
        RMS2,
        taille_image2,
        pattern_size2,
        square_size2,
        image_points2
    );

    if (camera_matrix1.empty() || dist_coeffs1.empty() ||
        camera_matrix2.empty() || dist_coeffs2.empty())
    {
        std::cerr << "Erreur : calibration mono non chargee correctement." << std::endl;
        return;
    }

    if (taille_image1 != taille_image2)
    {
        std::cerr << "Erreur : tailles d'image differentes entre les deux cameras." << std::endl;
        return;
    }

    if (pattern_size1 != pattern_size2)
    {
        std::cerr << "Erreur : pattern_size different entre les deux cameras." << std::endl;
        return;
    }

    if (square_size1 != square_size2)
    {
        std::cerr << "Erreur : square_size different entre les deux cameras." << std::endl;
        return;
    }

    if (image_points1.empty() || image_points2.empty())
    {
        std::cerr << "Erreur : image_points vide." << std::endl;
        return;
    }

    if (image_points1.size() != image_points2.size())
    {
        std::cerr << "Erreur : nombre de vues different entre les deux cameras." << std::endl;
        std::cerr << "Cam0 : " << image_points1.size() << std::endl;
        std::cerr << "Cam1 : " << image_points2.size() << std::endl;
        return;
    }

    int nb_points = pattern_size1.width * pattern_size1.height;

    for (size_t i = 0; i < image_points1.size(); ++i)
    {
        if (image_points1[i].size() != static_cast<size_t>(nb_points) ||
            image_points2[i].size() != static_cast<size_t>(nb_points))
        {
            std::cerr << "Erreur : nombre de coins invalide a la vue " << i << std::endl;
            std::cerr << "Cam0 : " << image_points1[i].size() << std::endl;
            std::cerr << "Cam1 : " << image_points2[i].size() << std::endl;
            return;
        }
    }

    std::vector<cv::Point3f> pattern_points;
    pattern_points.reserve(nb_points);

    for (int y = 0; y < pattern_size1.height; ++y)
    {
        for (int x = 0; x < pattern_size1.width; ++x)
        {
            pattern_points.push_back(
                cv::Point3f(
                    x * square_size1,
                    y * square_size1,
                    0.0f
                )
            );
        }
    }

    std::vector<std::vector<cv::Point3f>> object_points(image_points1.size(), pattern_points);

    cv::Mat R, T, E, F;

    double rmsStereo = cv::stereoCalibrate(
        object_points,
        image_points1,
        image_points2,
        camera_matrix1,
        dist_coeffs1,
        camera_matrix2,
        dist_coeffs2,
        taille_image1,
        R,
        T,
        E,
        F,
        cv::CALIB_FIX_INTRINSIC,
        cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-5)
    );

    std::cout << "Stereo calibration terminee." << std::endl;
    std::cout << "RMS stereo : " << rmsStereo << std::endl;

    std::cout << "R =" << std::endl;
    std::cout << R << std::endl;

    std::cout << "T =" << std::endl;
    std::cout << T << std::endl;

    std::cout << "E =" << std::endl;
    std::cout << E << std::endl;

    std::cout << "F =" << std::endl;
    std::cout << F << std::endl;
}