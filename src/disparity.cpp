#include "disparity.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

void show_disparity(
    const cv::Mat& left_rect,
    const cv::Mat& right_rect)
{
    if (left_rect.empty() || right_rect.empty())
    {
        std::cerr << "Erreur : une des images rectifiees est vide." << std::endl;
        return;
    }

    if (left_rect.size() != right_rect.size())
    {
        std::cerr << "Erreur : les deux images rectifiees n'ont pas la meme taille." << std::endl;
        return;
    }

    cv::Mat gray_left;
    cv::Mat gray_right;

    if (left_rect.channels() == 3)
    {
        cv::cvtColor(left_rect, gray_left, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray_left = left_rect.clone();
    }

    if (right_rect.channels() == 3)
    {
        cv::cvtColor(right_rect, gray_right, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray_right = right_rect.clone();
    }

    int min_disparity = 0;
    int num_disparities = 16 * 6; // doit etre un multiple de 16
    int block_size = 11;          // doit etre impair

    cv::Ptr<cv::StereoSGBM> stereo = cv::StereoSGBM::create(
        min_disparity,
        num_disparities,
        block_size);

    stereo->setP1(8 * block_size * block_size);
    stereo->setP2(32 * block_size * block_size);
    stereo->setDisp12MaxDiff(1);
    stereo->setUniquenessRatio(10);
    stereo->setSpeckleWindowSize(100);
    stereo->setSpeckleRange(32);
    stereo->setMode(cv::StereoSGBM::MODE_SGBM_3WAY);

    cv::Mat disparity_raw;
    stereo->compute(gray_left, gray_right, disparity_raw);

    if (disparity_raw.empty())
    {
        std::cerr << "Erreur : echec du calcul de la disparite." << std::endl;
        return;
    }

    cv::Mat disparity_8u;
    cv::Mat disparity_color;

    cv::normalize(disparity_raw, disparity_8u, 0, 255, cv::NORM_MINMAX, CV_8U);
    cv::applyColorMap(disparity_8u, disparity_color, cv::COLORMAP_JET);

    cv::namedWindow("Carte de disparite", cv::WINDOW_AUTOSIZE);
    cv::imshow("Carte de disparite", disparity_color);

    if (!cv::imwrite("disparity_map.png", disparity_color))
    {
        std::cerr << "Erreur : impossible de sauvegarder disparity_map.png" << std::endl;
    }
}