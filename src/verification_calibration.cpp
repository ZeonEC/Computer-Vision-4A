#include <iostream>
#include <opencv2/opencv.hpp>

#include "verification_calibration.hpp"

using namespace std;


// dessine une croix sur l'image
void drawCross(cv::Mat& img, cv::Point2f p, cv::Scalar color)
{
    int size = 6;

    cv::line(img,
        cv::Point(p.x - size, p.y - size),
        cv::Point(p.x + size, p.y + size),
        color, 2);

    cv::line(img,
        cv::Point(p.x - size, p.y + size),
        cv::Point(p.x + size, p.y - size),
        color, 2);
}


// verification de la calibration
void verification_calibration()
{
    // charger calibration
    string filename =
        "../../../calibration_images/results/calibration_cam1_result.xml";

    cv::FileStorage fs(filename, cv::FileStorage::READ);

    if (!fs.isOpened())
    {
        cout << "Impossible d'ouvrir le fichier XML\n";
        return;
    }

    cv::Mat camera_matrix;
    cv::Mat dist_coeffs;

    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;

    fs.release();

    cout << "Calibration chargee\n";


    // damier
    cv::Size pattern_size(7, 9);
    float square_size = 20.0f;


    // points 3D
    vector<cv::Point3f> obj;

    for (int i = 0; i < pattern_size.height; i++)
    {
        for (int j = 0; j < pattern_size.width; j++)
        {
            obj.push_back(
                cv::Point3f(
                    j * square_size,
                    i * square_size,
                    0));
        }
    }


    // charger image
    string image_path =
        "../../../calibration_images/calib1.png";

    cv::Mat image = cv::imread(image_path);

    if (image.empty())
    {
        cout << "Impossible de charger l'image\n";
        return;
    }


    // detecter coins
    vector<cv::Point2f> corners;

    bool found =
        cv::findChessboardCorners(
            image,
            pattern_size,
            corners);

    if (!found)
    {
        cout << "Damier non detecte\n";
        return;
    }

    cout << "Damier detecte\n";


    // pose du damier
    cv::Mat rvec, tvec;

    cv::solvePnP(
        obj,
        corners,
        camera_matrix,
        dist_coeffs,
        rvec,
        tvec);


    // reprojection
    vector<cv::Point2f> projected;

    cv::projectPoints(
        obj,
        rvec,
        tvec,
        camera_matrix,
        dist_coeffs,
        projected);


    // dessin
    for (size_t i = 0; i < corners.size(); i++)
    {
        drawCross(image, corners[i], cv::Scalar(0, 255, 0)); // détecté
        drawCross(image, projected[i], cv::Scalar(0, 0, 255)); // reprojeté
    }


    // erreur reprojection
    double error = 0;

    for (size_t i = 0; i < corners.size(); i++)
    {
        error += cv::norm(corners[i] - projected[i]);
    }

    error /= corners.size();

    cout << "Erreur moyenne = "
        << error << " pixels\n";


    // affichage
    cv::imshow("Verification calibration", image);

    cv::waitKey(0);

    cv::destroyAllWindows();
}