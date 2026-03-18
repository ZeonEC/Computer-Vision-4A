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
        "calibration_cam0.xml",
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
        "calibration_cam1.xml",
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

    // Vérifications de cohérence minimales
    if (camera_matrix1.empty() || dist_coeffs1.empty() ||
        camera_matrix2.empty() || dist_coeffs2.empty())
    {
        std::cerr << "Erreur : matrices intrinseques ou coefficients de distorsion vides." << std::endl;
        return;
    }

    if (taille_image1 != taille_image2)
    {
        std::cerr << "Erreur : les tailles d'image des deux cameras sont differentes." << std::endl;
        return;
    }

    if (pattern_size1 != pattern_size2)
    {
        std::cerr << "Erreur : les tailles de damier des deux cameras sont differentes." << std::endl;
        return;
    }

    if (square_size1 != square_size2)
    {
        std::cerr << "Erreur : les tailles de carre du damier sont differentes." << std::endl;
        return;
    }

    if (image_points1.empty() || image_points2.empty())
    {
        std::cerr << "Erreur : points image vides." << std::endl;
        return;
    }

    if (image_points1.size() != image_points2.size())
    {
        std::cerr << "Erreur : nombre de vues different entre les deux cameras." << std::endl;
        std::cerr << "camera 1 : " << image_points1.size() << " vues" << std::endl;
        std::cerr << "camera 2 : " << image_points2.size() << " vues" << std::endl;
        return;
    }

    const int nb_points_attendus = pattern_size1.width * pattern_size1.height;

    for (size_t i = 0; i < image_points1.size(); i++)
    {
        if (image_points1[i].size() != static_cast<size_t>(nb_points_attendus) ||
            image_points2[i].size() != static_cast<size_t>(nb_points_attendus))
        {
            std::cerr << "Erreur : nombre de coins invalide a la vue " << i << std::endl;
            std::cerr << "camera 1 : " << image_points1[i].size() << " points" << std::endl;
            std::cerr << "camera 2 : " << image_points2[i].size() << " points" << std::endl;
            return;
        }
    }

    // Construction des points 3D du damier
    std::vector<cv::Point3f> one_pattern_points;
    one_pattern_points.reserve(nb_points_attendus);

    for (int y = 0; y < pattern_size1.height; y++)
    {
        for (int x = 0; x < pattern_size1.width; x++)
        {
            one_pattern_points.push_back(
                cv::Point3f(
                    static_cast<float>(x) * square_size1,
                    static_cast<float>(y) * square_size1,
                    0.0f
                )
            );
        }
    }

    std::vector<std::vector<cv::Point3f>> objectPoints(image_points1.size(), one_pattern_points);

    cv::Mat R, T, E, F;

    double rms = cv::stereoCalibrate(
        objectPoints,
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
        cv::TermCriteria(
            cv::TermCriteria::COUNT + cv::TermCriteria::EPS,
            100,
            1e-5
        )
    );

    std::cout << "Stereo calibration terminee." << std::endl;
    std::cout << "RMS stereo = " << rms << std::endl;

    std::cout << "Matrice R :" << std::endl;
    std::cout << R << std::endl;

    std::cout << "Vecteur T :" << std::endl;
    std::cout << T << std::endl;

    std::cout << "Matrice E :" << std::endl;
    std::cout << E << std::endl;

    std::cout << "Matrice F :" << std::endl;
    std::cout << F << std::endl;
}