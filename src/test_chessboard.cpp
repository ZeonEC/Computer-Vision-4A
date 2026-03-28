#include "test_chessboard.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

// =======================
// DESSIN DES POINTS + INDEX
// =======================
static void drawOrderedCorners(cv::Mat& img, const std::vector<cv::Point2f>& corners)
{
    for (size_t i = 0; i < corners.size(); i++)
    {
        // point
        cv::circle(img, corners[i], 3, cv::Scalar(0, 0, 255), -1);

        // numéro
        cv::putText(img,
            std::to_string(i),
            corners[i],
            cv::FONT_HERSHEY_SIMPLEX,
            0.4,
            cv::Scalar(255, 0, 0),
            1);
    }
}

// =======================
// PROGRAMME PRINCIPAL
// =======================
void test_order_chessboard()
{
    cv::Size patternSize(7, 9); //  

    int i = 1;

    while (true)
    {
        std::string img0_path = "../../../calibration_images/calib_cam0_" + std::to_string(i) + ".png";
        std::string img1_path = "../../../calibration_images/calib_cam1_" + std::to_string(i) + ".png";

        cv::Mat img0 = cv::imread(img0_path);
        cv::Mat img1 = cv::imread(img1_path);

        if (img0.empty() || img1.empty())
        {
            std::cout << "Fin des images ou erreur chargement." << std::endl;
            break;
        }

        std::vector<cv::Point2f> corners0, corners1;

        bool found0 = cv::findChessboardCorners(img0, patternSize, corners0);
        bool found1 = cv::findChessboardCorners(img1, patternSize, corners1);

        if (!found0 || !found1)
        {
            std::cout << "Damier non detecte image " << i << std::endl;
            i++;
            continue;
        }

        // amélioration précision
        cv::Mat gray0, gray1;
        cv::cvtColor(img0, gray0, cv::COLOR_BGR2GRAY);
        cv::cvtColor(img1, gray1, cv::COLOR_BGR2GRAY);

        cv::cornerSubPix(gray0, corners0, cv::Size(11, 11), cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

        cv::cornerSubPix(gray1, corners1, cv::Size(11, 11), cv::Size(-1, -1),
            cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

        // dessin ordre
        drawOrderedCorners(img0, corners0);
        drawOrderedCorners(img1, corners1);

        // concat gauche/droite
        cv::Mat combined;
        cv::hconcat(img0, img1, combined);

        cv::imshow("Verification ordre corners", combined);

        std::cout << "Image " << i << " | n = suivante | ESC = quitter" << std::endl;

        int key = cv::waitKey(0);

        if (key == 27) break;
        if (key == 'n') i++;
    }

    cv::destroyAllWindows();
}