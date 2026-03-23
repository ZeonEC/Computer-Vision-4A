#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <opencv2/opencv.hpp>

#include "verif_stereo_calib.hpp"

// =======================
// ZOOM GLOBAL
// =======================
static double zoom = 1.0;
static cv::Mat image_original;

static int mouse_x = 0;
static int mouse_y = 0;

// =======================
// CALLBACK SOURIS (zoom)
// =======================
void onMouseStereo(int event, int x, int y, int flags, void*)
{
    mouse_x = x;
    mouse_y = y;

    if (event == cv::EVENT_MOUSEWHEEL)
    {
        if (flags > 0)
            zoom *= 1.2;
        else
            zoom /= 1.2;

        zoom = std::max(1.0, std::min(zoom, 10.0));
    }
}

// =======================
// CROIX
// =======================
void drawCrossStereo(cv::Mat& image, const cv::Point2f& pt, const cv::Scalar& color)
{
    int size = 2;

    cv::line(image,
        cv::Point(static_cast<int>(pt.x - size), static_cast<int>(pt.y - size)),
        cv::Point(static_cast<int>(pt.x + size), static_cast<int>(pt.y + size)),
        color, 1);

    cv::line(image,
        cv::Point(static_cast<int>(pt.x - size), static_cast<int>(pt.y + size)),
        cv::Point(static_cast<int>(pt.x + size), static_cast<int>(pt.y - size)),
        color, 1);
}

// =======================
// DROITE EPIPOLAIRE
// =======================
void drawEpipolarLineStereo(cv::Mat& img, const cv::Vec3f& line, const cv::Scalar& color)
{
    double a = line[0];
    double b = line[1];
    double c = line[2];

    if (std::abs(b) < 1e-9)
        return;

    cv::Point p1(0, static_cast<int>(-c / b));
    cv::Point p2(img.cols, static_cast<int>(-(c + a * img.cols) / b));

    cv::line(img, p1, p2, color, 1);
}

// =======================
// LECTURE D'UNE MATRICE DE VECTEURS
// =======================
bool readMatVectorStereo(const cv::FileNode& node, std::vector<cv::Mat>& vec)
{
    vec.clear();

    if (node.empty())
        return false;

    for (cv::FileNodeIterator it = node.begin(); it != node.end(); ++it)
    {
        cv::Mat m;
        (*it) >> m;
        vec.push_back(m);
    }

    return true;
}

// =======================
// LECTURE D'UN TABLEAU DE POINTS 2D
// =======================
bool readPoint2fVectorVectorStereo(
    const cv::FileNode& node,
    std::vector<std::vector<cv::Point2f>>& points)
{
    points.clear();

    if (node.empty())
        return false;

    for (cv::FileNodeIterator it = node.begin(); it != node.end(); ++it)
    {
        std::vector<cv::Point2f> pts;
        (*it) >> pts;
        points.push_back(pts);
    }

    return true;
}

// =======================
// CHARGEMENT CALIBRATION MONO
// =======================
bool loadCalibrationXmlStereo(
    const std::string& filename,
    cv::Mat& camera_matrix,
    cv::Mat& dist_coeffs,
    std::vector<cv::Mat>& rvecs,
    std::vector<cv::Mat>& tvecs,
    cv::Size& image_size,
    cv::Size& pattern_size,
    float& square_size,
    std::vector<std::vector<cv::Point2f>>& image_points)
{
    cv::FileStorage fs(filename, cv::FileStorage::READ);

    if (!fs.isOpened())
    {
        std::cout << "Erreur ouverture fichier : " << filename << std::endl;
        return false;
    }

    fs["camera_matrix"] >> camera_matrix;
    fs["dist_coeffs"] >> dist_coeffs;

    readMatVectorStereo(fs["rvecs"], rvecs);
    readMatVectorStereo(fs["tvecs"], tvecs);

    int image_width = 0;
    int image_height = 0;
    fs["image_width"] >> image_width;
    fs["image_height"] >> image_height;
    image_size = cv::Size(image_width, image_height);

    int pattern_width = 0;
    int pattern_height = 0;
    fs["pattern_width"] >> pattern_width;
    fs["pattern_height"] >> pattern_height;
    pattern_size = cv::Size(pattern_width, pattern_height);

    fs["square_size"] >> square_size;

    readPoint2fVectorVectorStereo(fs["image_points"], image_points);

    fs.release();

    if (camera_matrix.empty() || dist_coeffs.empty() || image_points.empty())
    {
        std::cout << "Donnees manquantes dans : " << filename << std::endl;
        return false;
    }

    return true;
}

// =======================
// CHARGEMENT MATRICE F
// =======================
bool loadFundamentalMatrixStereo(const std::string& filename, cv::Mat& F)
{
    cv::FileStorage fs(filename, cv::FileStorage::READ);

    if (!fs.isOpened())
    {
        std::cout << "Erreur ouverture fichier : " << filename << std::endl;
        return false;
    }

    fs["F"] >> F;
    fs.release();

    if (F.empty())
    {
        std::cout << "Matrice F absente dans : " << filename << std::endl;
        return false;
    }

    return true;
}

// =======================
// VERIFICATION
// =======================
void verif_projection_stereo(int& nb_cam)
{
    if (nb_cam < 2)
    {
        std::cout << "Mode stereo requis (2 cameras)" << std::endl;
        return;
    }

    cv::Mat camera_matrix0, dist_coeffs0;
    std::vector<cv::Mat> rvecs0, tvecs0;
    cv::Size taille_image0, pattern_size0;
    float square_size0 = 0.0f;
    std::vector<std::vector<cv::Point2f>> image_points0;

    if (!loadCalibrationXmlStereo(
        "../../../calibration_images/results/calibration_cam0_result.xml",
        camera_matrix0, dist_coeffs0, rvecs0, tvecs0,
        taille_image0, pattern_size0, square_size0, image_points0))
    {
        return;
    }

    cv::Mat camera_matrix1, dist_coeffs1;
    std::vector<cv::Mat> rvecs1, tvecs1;
    cv::Size taille_image1, pattern_size1;
    float square_size1 = 0.0f;
    std::vector<std::vector<cv::Point2f>> image_points1;

    if (!loadCalibrationXmlStereo(
        "../../../calibration_images/results/calibration_cam1_result.xml",
        camera_matrix1, dist_coeffs1, rvecs1, tvecs1,
        taille_image1, pattern_size1, square_size1, image_points1))
    {
        return;
    }

    if (image_points0.size() != image_points1.size())
    {
        std::cout << "Nombre d'images incoherent entre cam0 et cam1" << std::endl;
        return;
    }

    if (rvecs0.size() != image_points0.size() || tvecs0.size() != image_points0.size())
    {
        std::cout << "Nombre de poses incoherent pour cam0" << std::endl;
        return;
    }

    if (rvecs1.size() != image_points1.size() || tvecs1.size() != image_points1.size())
    {
        std::cout << "Nombre de poses incoherent pour cam1" << std::endl;
        return;
    }

    cv::Mat F;
    if (!loadFundamentalMatrixStereo(
        "../../../calibration_images/results/stereo_result.xml", F))
    {
        return;
    }

    std::vector<cv::Point3f> obj;
    std::vector<std::vector<cv::Point3f>> object_points;

    for (int i = 0; i < pattern_size0.height; i++)
    {
        for (int j = 0; j < pattern_size0.width; j++)
        {
            obj.push_back(cv::Point3f(
                static_cast<float>(j * square_size0),
                static_cast<float>(i * square_size0),
                0.0f));
        }
    }

    for (size_t i = 0; i < image_points0.size(); i++)
    {
        object_points.push_back(obj);
    }

    for (size_t i = 0; i < image_points0.size(); i++)
    {
        std::string img0_name =
            "../../../calibration_images/calib_cam0_" + std::to_string(i + 1) + ".png";

        std::string img1_name =
            "../../../calibration_images/calib_cam1_" + std::to_string(i + 1) + ".png";

        cv::Mat img0 = cv::imread(img0_name);
        cv::Mat img1 = cv::imread(img1_name);

        if (img0.empty() || img1.empty())
        {
            std::cout << "Erreur chargement image " << i + 1 << std::endl;
            continue;
        }

        std::vector<cv::Point2f> proj0, proj1;

        cv::projectPoints(object_points[i], rvecs0[i], tvecs0[i],
            camera_matrix0, dist_coeffs0, proj0);

        cv::projectPoints(object_points[i], rvecs1[i], tvecs1[i],
            camera_matrix1, dist_coeffs1, proj1);

        std::vector<cv::Vec3f> lignes1, lignes2;

        cv::computeCorrespondEpilines(image_points0[i], 1, F, lignes2);
        cv::computeCorrespondEpilines(image_points1[i], 2, F, lignes1);

        size_t nb_points = std::min(image_points0[i].size(), image_points1[i].size());

        for (size_t j = 0; j < nb_points; j++)
        {
            drawCrossStereo(img0, image_points0[i][j], cv::Scalar(0, 255, 0));
            drawCrossStereo(img1, image_points1[i][j], cv::Scalar(0, 255, 0));

            if (j < proj0.size())
                drawCrossStereo(img0, proj0[j], cv::Scalar(0, 0, 255));
            if (j < proj1.size())
                drawCrossStereo(img1, proj1[j], cv::Scalar(0, 0, 255));

            if (j < lignes2.size())
                drawEpipolarLineStereo(img1, lignes2[j], cv::Scalar(255, 0, 0));
            if (j < lignes1.size())
                drawEpipolarLineStereo(img0, lignes1[j], cv::Scalar(255, 0, 0));
        }

        cv::Mat combined;
        cv::hconcat(img0, img1, combined);

        image_original = combined.clone();
        zoom = 1.0;
        mouse_x = image_original.cols / 2;
        mouse_y = image_original.rows / 2;

        cv::namedWindow("Verification calibration", cv::WINDOW_NORMAL);
        cv::setMouseCallback("Verification calibration", onMouseStereo);

        while (true)
        {
            cv::Mat display;

            int w = image_original.cols;
            int h = image_original.rows;

            int new_w = static_cast<int>(w / zoom);
            int new_h = static_cast<int>(h / zoom);

            new_w = std::max(1, std::min(new_w, w));
            new_h = std::max(1, std::min(new_h, h));

            int x = mouse_x - new_w / 2;
            int y = mouse_y - new_h / 2;

            x = std::max(0, std::min(x, w - new_w));
            y = std::max(0, std::min(y, h - new_h));

            cv::Rect roi(x, y, new_w, new_h);
            cv::Mat cropped = image_original(roi);

            cv::resize(cropped, display, cv::Size(w, h));
            cv::imshow("Verification calibration", display);

            int key = cv::waitKey(30);

            if (key == 27)
                return;
            if (key == 'n')
                break;
        }

        cv::destroyAllWindows();
    }
}

// =======================
// MAIN WRAPPER
// =======================
void verification_calibration_stereo()
{
    int nb_cam = 2;
    verif_projection(nb_cam);
}