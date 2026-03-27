#include "raw_geometrie_epipolaire.hpp"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// Variables globales du fichier
cv::Mat img_gauche_og;
cv::Mat img_droite_og;
cv::Mat cur_imgleft;
cv::Mat cur_imgright;
cv::Mat Fondamentale;

std::string windowLeft;
std::string windowRight;

// Dessin d'une droite epipolaire ax + by + c = 0
void drawEpipolarLine(cv::Mat& img, const cv::Vec3f& line, const cv::Scalar& color)
{
    float a = line[0];
    float b = line[1];
    float c = line[2];

    cv::Point p1, p2;

    if (std::abs(b) > 1e-6f)
    {
        p1 = cv::Point(0, cvRound(-c / b));
        p2 = cv::Point(img.cols - 1, cvRound(-(c + a * (img.cols - 1)) / b));
    }
    else if (std::abs(a) > 1e-6f)
    {
        int x = cvRound(-c / a);
        p1 = cv::Point(x, 0);
        p2 = cv::Point(x, img.rows - 1);
    }
    else
    {
        return;
    }

    cv::line(img, p1, p2, color, 2);
}

// Reset affichage
void resetDisplays()
{
	// l'image actuelle est une copie de l'originale pour pouvoir dessiner dessus
    cur_imgleft = img_gauche_og.clone();
    cur_imgright = img_droite_og.clone();

    cv::imshow(windowLeft, cur_imgleft);
    cv::imshow(windowRight, cur_imgright);
}

// Clique image gauche -> calcule droite dans image droite
void onMouseLeft(int event, int x, int y, int, void*)
{
    if (event != cv::EVENT_LBUTTONDOWN)
    {
        return;
    }

    resetDisplays();

    cv::Point2f clickedPoint(static_cast<float>(x), static_cast<float>(y));
    cv::circle(cur_imgleft, clickedPoint, 5, cv::Scalar(0, 0, 255), -1);

    std::vector<cv::Point2f> pointsLeft(1, clickedPoint);
    std::vector<cv::Vec3f> epilinesRight;

    cv::computeCorrespondEpilines(pointsLeft, 1, Fondamentale, epilinesRight);

    if (!epilinesRight.empty())
    {
        drawEpipolarLine(cur_imgright, epilinesRight[0], cv::Scalar(0, 255, 0));
    }

    cv::imshow(windowLeft, cur_imgleft);
    cv::imshow(windowRight, cur_imgright);

    std::cout << "[GAUCHE] Point clique : (" << x << ", " << y << ")" << std::endl;
}

// -----------------------------------------------------------------------------
// Clique image droite -> calcule droite dans image gauche
// -----------------------------------------------------------------------------
static void onMouseRight(int event, int x, int y, int, void*)
{
    if (event != cv::EVENT_LBUTTONDOWN)
    {
        return;
    }

    resetDisplays();

    cv::Point2f clickedPoint(static_cast<float>(x), static_cast<float>(y));
    cv::circle(cur_imgright, clickedPoint, 5, cv::Scalar(0, 0, 255), -1);

    std::vector<cv::Point2f> pointsRight(1, clickedPoint);
    std::vector<cv::Vec3f> epilinesLeft;

    cv::computeCorrespondEpilines(pointsRight, 2, Fondamentale, epilinesLeft);

    if (!epilinesLeft.empty())
    {
        drawEpipolarLine(cur_imgleft, epilinesLeft[0], cv::Scalar(0, 255, 0));
    }

    cv::imshow(windowLeft, cur_imgleft);
    cv::imshow(windowRight, cur_imgright);

    std::cout << "[DROITE] Point clique : (" << x << ", " << y << ")" << std::endl;
}


void raw_geometrie_epipolaire(
    const cv::Mat& img_gauche,
    const cv::Mat& img_droite,
    const cv::Mat& F)
{
    if (img_gauche.empty() || img_droite.empty())
    {
        std::cerr << "Erreur : une ou plusieurs images sont vides." << std::endl;
        return;
    }

    if (F.empty() || F.rows != 3 || F.cols != 3)
    {
        std::cerr << "Erreur : la matrice fondamentale F est invalide." << std::endl;
        return;
    }

    img_gauche_og = img_gauche.clone();
    img_droite_og = img_gauche.clone();
   	Fondamentale = F.clone();

    windowLeft = "Camera de gauche";
    windowRight = "Camera de droite";

    cv::namedWindow(windowLeft, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(windowRight, cv::WINDOW_AUTOSIZE);

    resetDisplays();

    cv::setMouseCallback(windowLeft, onMouseLeft);
    cv::setMouseCallback(windowRight, onMouseRight);

    std::cout << "=== Geometrie epipolaire brute ===" << std::endl;
    std::cout << "Clic gauche dans une image : trace la droite epipolaire dans l'autre." << std::endl;
    std::cout << "Touche 'r' : reset affichage" << std::endl;
    std::cout << "Touche 'q' ou ESC : quitter" << std::endl;

    while (true)
    {
        int key = cv::waitKey(30);

        if (key == 27 || key == 'q' || key == 'Q')
        {
            break;
        }
        else if (key == 'r' || key == 'R')
        {
            resetDisplays();
        }
    }

    cv::destroyWindow(windowLeft);
    cv::destroyWindow(windowRight);
}