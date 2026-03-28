#include "stereo_calibration.hpp"
#include "gestion_XML.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

// =======================
// NORMALISATION DAMIER
// =======================
void normalizeChessboard(std::vector<cv::Point2f>& pts, cv::Size patternSize)
{
    if (pts.size() != static_cast<size_t>(patternSize.width * patternSize.height))
        return;

    // vecteur horizontal
    cv::Point2f dx = pts[1] - pts[0];

    // vecteur vertical
    cv::Point2f dy = pts[patternSize.width] - pts[0];

    // inversion horizontale (ligne inversée)
    if (dx.x < 0)
    {
        for (int y = 0; y < patternSize.height; y++)
        {
            std::reverse(
                pts.begin() + y * patternSize.width,
                pts.begin() + (y + 1) * patternSize.width
            );
        }
    }

    // recalcul après correction
    dx = pts[1] - pts[0];
    dy = pts[patternSize.width] - pts[0];

    // inversion verticale (damier retourné)
    if (dy.y < 0)
    {
        std::vector<cv::Point2f> temp = pts;

        for (int y = 0; y < patternSize.height; y++)
        {
            for (int x = 0; x < patternSize.width; x++)
            {
                pts[y * patternSize.width + x] =
                    temp[(patternSize.height - 1 - y) * patternSize.width + x];
            }
        }
    }
}

// =======================
// CALIBRATION STEREO
// =======================
void stereo_calibration()
{
    std::cout << "STEREO RUNNING" << std::endl;

    // =======================
    // VARIABLES
    // =======================
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

    // =======================
    // CHARGEMENT XML MONO
    // =======================
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

    // =======================
    // VERIFS
    // =======================
    if (camera_matrix1.empty() || camera_matrix2.empty())
        return;

    if (image_points1.empty() || image_points2.empty())
        return;

    if (image_points1.size() != image_points2.size())
        return;

    // =======================
    // 🔥 CORRECTION DES POINTS
    // =======================
    for (size_t i = 0; i < image_points1.size(); i++)
    {
        normalizeChessboard(image_points1[i], pattern_size1);
        normalizeChessboard(image_points2[i], pattern_size2);
    }

    // =======================
    // OBJECT POINTS
    // =======================
    std::vector<cv::Point3f> pattern_points;

    for (int y = 0; y < pattern_size1.height; y++)
    {
        for (int x = 0; x < pattern_size1.width; x++)
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

    std::vector<std::vector<cv::Point3f>> object_points(
        image_points1.size(), pattern_points
    );

    // =======================
    // CALCUL STEREO
    // =======================
    cv::Mat R, T, E, F;

    double rms = cv::stereoCalibrate(
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
        cv::CALIB_FIX_INTRINSIC
    );

    std::cout << "RMS stereo = " << rms << std::endl;

    // =======================
    // SAUVEGARDE
    // =======================
    cv::FileStorage fs(
        "../../../calibration_images/results/stereo_result.xml",
        cv::FileStorage::WRITE
    );

    if (!fs.isOpened())
    {
        std::cerr << "Erreur creation stereo_result.xml" << std::endl;
        return;
    }

    fs << "R" << R;
    fs << "T" << T;
    fs << "E" << E;
    fs << "F" << F;

    fs.release();

    std::cout << "stereo_result.xml cree !" << std::endl;
}