#include "stereo_calibration.hpp"
#include "gestion_XML.hpp"

#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>
#include "verif_stereo_calib.hpp"
#include "calibration.hpp"

void reverse_chessboard_x(std::vector<cv::Point2f>& corners, const cv::Size& pattern_size)
{
    std::vector<cv::Point2f> reordered(corners.size());

    for (int y = 0; y < pattern_size.height; y++)
    {
        for (int x = 0; x < pattern_size.width; x++)
        {
            int src = y * pattern_size.width + x;
            int dst = y * pattern_size.width + (pattern_size.width - 1 - x);
            reordered[dst] = corners[src];
        }
    }

    corners = reordered;
}

void reverse_chessboard_y(std::vector<cv::Point2f>& corners, const cv::Size& pattern_size)
{
    std::vector<cv::Point2f> reordered(corners.size());

    for (int y = 0; y < pattern_size.height; y++)
    {
        for (int x = 0; x < pattern_size.width; x++)
        {
            int src = y * pattern_size.width + x;
            int dst = (pattern_size.height - 1 - y) * pattern_size.width + x;
            reordered[dst] = corners[src];
        }
    }

    corners = reordered;
}

void draw_corner_indices(cv::Mat& image, const std::vector<cv::Point2f>& corners, const cv::Size& pattern_size)
{
    if (corners.empty())
        return;

    std::vector<int> ids_to_draw;

    ids_to_draw.push_back(0);

    if (pattern_size.width > 1)
        ids_to_draw.push_back(1);

    if (pattern_size.width > 2)
        ids_to_draw.push_back(pattern_size.width - 1);

    if (pattern_size.height > 1)
        ids_to_draw.push_back(pattern_size.width);

    if (pattern_size.height > 1 && pattern_size.width > 1)
        ids_to_draw.push_back(pattern_size.width + 1);

    if (pattern_size.height > 1)
        ids_to_draw.push_back((pattern_size.height - 1) * pattern_size.width);

    for (int idx : ids_to_draw)
    {
        if (idx >= 0 && idx < static_cast<int>(corners.size()))
        {
            cv::putText(
                image,
                std::to_string(idx),
                corners[idx] + cv::Point2f(5.0f, -5.0f),
                cv::FONT_HERSHEY_SIMPLEX,
                0.5,
                cv::Scalar(255, 0, 0),
                1
            );
        }
    }
}

void get_stereo_grid_points(
    int& nb_calib,
    std::vector<std::vector<cv::Point2f>>& image_points0,
    std::vector<std::vector<cv::Point2f>>& image_points1)
{
    cv::Size pattern_size(4, 5); // nombre de coins interieurs dans la grille (7x9)

    image_points0.clear();
    image_points1.clear();

    for (int i = 1; i <= nb_calib; i++)
    {
        std::string filename_cam0 = "../../../calibration_images/calib_cam0_" + std::to_string(i) + ".png";
        std::string filename_cam1 = "../../../calibration_images/calib_cam1_" + std::to_string(i) + ".png";

        cv::Mat image_cam0 = cv::imread(filename_cam0);
        cv::Mat image_cam1 = cv::imread(filename_cam1);

        if (image_cam0.empty() || image_cam1.empty())
        {
            std::cerr << "Erreur chargement images stereo numero " << i << std::endl;
            continue;
        }

        std::vector<cv::Point2f> corners_cam0_raw;
        std::vector<cv::Point2f> corners_cam1_raw;

        bool found_cam0 = cv::findChessboardCorners(image_cam0, pattern_size, corners_cam0_raw);
        bool found_cam1 = cv::findChessboardCorners(image_cam1, pattern_size, corners_cam1_raw);

        if (!found_cam0 || !found_cam1)
        {
            std::cout << "Image " << i << " ignoree : coins non trouves sur les deux cameras." << std::endl;
            continue;
        }

        cv::Mat gray0, gray1;
        cv::cvtColor(image_cam0, gray0, cv::COLOR_BGR2GRAY);
        cv::cvtColor(image_cam1, gray1, cv::COLOR_BGR2GRAY);

        cv::cornerSubPix(
            gray0,
            corners_cam0_raw,
            cv::Size(11, 11),
            cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1)
        );

        cv::cornerSubPix(
            gray1,
            corners_cam1_raw,
            cv::Size(11, 11),
            cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1)
        );

        bool reverse_cam1_x = false;
        bool reverse_cam1_y = false;

        while (true)
        {
            cv::Mat display_cam0 = image_cam0.clone();
            cv::Mat display_cam1 = image_cam1.clone();

            std::vector<cv::Point2f> corners_cam0 = corners_cam0_raw;
            std::vector<cv::Point2f> corners_cam1 = corners_cam1_raw;

            if (reverse_cam1_x)
                reverse_chessboard_x(corners_cam1, pattern_size);

            if (reverse_cam1_y)
                reverse_chessboard_y(corners_cam1, pattern_size);

            cv::drawChessboardCorners(display_cam0, pattern_size, corners_cam0, true);
            cv::drawChessboardCorners(display_cam1, pattern_size, corners_cam1, true);

            draw_corner_indices(display_cam0, corners_cam0, pattern_size);
            draw_corner_indices(display_cam1, corners_cam1, pattern_size);

            cv::putText(display_cam0, "cam0", cv::Point(20, 40),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

            std::string info_cam1 = "cam1";
            if (reverse_cam1_x) info_cam1 += " RX";
            if (reverse_cam1_y) info_cam1 += " RY";

            cv::putText(display_cam1, info_cam1, cv::Point(20, 40),
                cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2);

            cv::imshow("Stereo cam0 --- img" + std::to_string(i), display_cam0);
            cv::imshow("Stereo cam1 --- img" + std::to_string(i), display_cam1);

            cv::moveWindow("Stereo cam0 --- img" + std::to_string(i), 50, 50);
            cv::moveWindow("Stereo cam1 --- img" + std::to_string(i), 800, 50);

            int key = cv::waitKey(0);

            if (key == 27) // ESC
            {
                cv::destroyAllWindows();
                return;
            }
            else if (key == 'r' || key == 'R')
            {
                reverse_cam1_x = !reverse_cam1_x;
            }
            else if (key == 't' || key == 'T')
            {
                reverse_cam1_y = !reverse_cam1_y;
            }
            else if (key == 'v' || key == 'V')
            {
                if (reverse_cam1_x)
                    reverse_chessboard_x(corners_cam1_raw, pattern_size);

                if (reverse_cam1_y)
                    reverse_chessboard_y(corners_cam1_raw, pattern_size);

                image_points0.push_back(corners_cam0_raw);
                image_points1.push_back(corners_cam1_raw);

                std::cout << "Image " << i << " validee et sauvegardee." << std::endl;
                break;
            }
            else if (key == 's' || key == 'S')
            {
                std::cout << "Image " << i << " ignoree manuellement." << std::endl;
                break;
            }
        }

        cv::destroyWindow("Stereo cam0 --- img" + std::to_string(i));
        cv::destroyWindow("Stereo cam1 --- img" + std::to_string(i));
    }
}

void stereo_calibration(int &nb_calib)
{
    std::vector<std::vector<cv::Point2f>> image_points0, image_points1;
    get_stereo_grid_points(nb_calib, image_points0, image_points1);

    // taille de l'image
	cv::Size taille_image(600, 600);
    for (int i = 0; i < 2; i++)
    {
		if (i == 0)
            calibration(image_points0,taille_image,i);
        else
        calibration(image_points1,taille_image,i);
    }
    
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

    std::vector<std::vector<cv::Point2f>> image_points_1, image_points_2;

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
        image_points_1
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
        image_points_2
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

    if (image_points_1.empty() || image_points_2.empty())
    {
        std::cerr << "Erreur : image_points vide." << std::endl;
        return;
    }

    if (image_points_1.size() != image_points_2.size())
    {
        std::cerr << "Erreur : nombre de vues different entre les deux cameras." << std::endl;
        std::cerr << "Cam0 : " << image_points_1.size() << std::endl;
        std::cerr << "Cam1 : " << image_points_2.size() << std::endl;
        return;
    }

    int nb_points = pattern_size1.height * pattern_size1.width;

    for (size_t i = 0; i < image_points1.size(); ++i)
    {
        if (image_points_1[i].size() != static_cast<size_t>(nb_points) ||
            image_points_2[i].size() != static_cast<size_t>(nb_points))
        {
            std::cerr << "Erreur : nombre de coins invalide a la vue " << i << std::endl;
            std::cerr << "Cam0 : " << image_points_1[i].size() << std::endl;
            std::cerr << "Cam1 : " << image_points_2[i].size() << std::endl;
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

    std::vector<std::vector<cv::Point3f>> object_points(image_points_1.size(), pattern_points);

    cv::Mat R, T, E, F;

	// peut modifier les matrices K donc on met CALIB_FIX_INTRINSIC pour les garder fixes
    double rmsStereo = cv::stereoCalibrate(
        object_points,
        image_points_1,
        image_points_2,
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
    std::string XML_filename = "../../../calibration_images/results/stereo_calibration_result.xml";
    save_stereo_calibration_xml(
        XML_filename,
        camera_matrix1,
        dist_coeffs1,
        camera_matrix2,
        dist_coeffs2,
        R, T, E, F,
        rmsStereo
    );
}

