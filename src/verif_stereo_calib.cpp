#include <iostream>
#include <opencv2/opencv.hpp>

#include "verif_stereo_calib.hpp"
#include "gestion_XML.hpp"

#include <fstream>

// =======================
// ZOOM GLOBAL
// =======================
double zoom = 1.0;
cv::Mat image_original;

int mouse_x = 0;
int mouse_y = 0;

// =======================
// CALLBACK SOURIS (zoom)
// =======================

void onMouse(int event, int x, int y, int flags, void*)
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
void drawCross(cv::Mat& image, cv::Point2f pt, cv::Scalar color)
{
    int size = 2;

    cv::line(image,
        cv::Point(pt.x - size, pt.y - size),
        cv::Point(pt.x + size, pt.y + size),
        color, 1);

    cv::line(image,
        cv::Point(pt.x - size, pt.y + size),
        cv::Point(pt.x + size, pt.y - size),
        color, 1);
}

// =======================
// VERIFICATION
// =======================
void verif_projection(int& nb_cam)
{
    if (nb_cam < 2)
    {
        std::cout << "Mode stereo requis (2 cameras)" << std::endl;
        return;
    }

    // ===== LOAD XML CAM 0 =====
    cv::Mat camera_matrix0, dist_coeffs0;
    std::vector<cv::Mat> rvecs0, tvecs0;
    std::vector<double> perViewErrors0;
    double RMS0;
    cv::Size taille_image0, pattern_size0;
    float square_size0;
    std::vector<std::vector<cv::Point2f>> image_points0;

    get_calibration_from_xml(
        "../../../calibration_images/results/calibration_cam0_result.xml",
        camera_matrix0, dist_coeffs0, rvecs0, tvecs0,
        perViewErrors0, RMS0, taille_image0,
        pattern_size0, square_size0, image_points0
    );

    // ===== LOAD XML CAM 1 =====
    cv::Mat camera_matrix1, dist_coeffs1;
    std::vector<cv::Mat> rvecs1, tvecs1;
    std::vector<double> perViewErrors1;
    double RMS1;
    cv::Size taille_image1, pattern_size1;
    float square_size1;
    std::vector<std::vector<cv::Point2f>> image_points1;

    get_calibration_from_xml(
        "../../../calibration_images/results/calibration_cam1_result.xml",
        camera_matrix1, dist_coeffs1, rvecs1, tvecs1,
        perViewErrors1, RMS1, taille_image1,
        pattern_size1, square_size1, image_points1
    );

    // ===== OBJECT POINTS =====
    std::vector<cv::Point3f> obj;
    std::vector<std::vector<cv::Point3f>> object_points;

    for (int i = 0; i < pattern_size0.height; i++)
        for (int j = 0; j < pattern_size0.width; j++)
            obj.push_back(cv::Point3f(i * square_size0, j * square_size0, 0.0f));

    for (size_t i = 0; i < image_points0.size(); i++)
        object_points.push_back(obj);

    // ===== LOOP =====
    for (int i = 0; i < image_points0.size(); i++)
    {
        std::string img0_name =
            "../../../calibration_images/calib_cam0_" + std::to_string(i + 1) + ".png";

        std::string img1_name =
            "../../../calibration_images/calib_cam1_" + std::to_string(i + 1) + ".png";

        cv::Mat img0 = cv::imread(img0_name);
        cv::Mat img1 = cv::imread(img1_name);

        if (img0.empty() || img1.empty())
        {
            std::cout << "Erreur chargement image " << i << std::endl;
            continue;
        }

        // ===== PROJECTION =====
        std::vector<cv::Point2f> proj0, proj1;

        cv::projectPoints(object_points[i], rvecs0[i], tvecs0[i],
            camera_matrix0, dist_coeffs0, proj0);

        cv::projectPoints(object_points[i], rvecs1[i], tvecs1[i],
            camera_matrix1, dist_coeffs1, proj1);

        // ===== DRAW =====
        for (int j = 0; j < image_points0[i].size(); j++)
        {
            drawCross(img0, image_points0[i][j], cv::Scalar(0, 255, 0));
            drawCross(img0, proj0[j], cv::Scalar(0, 0, 255));

            drawCross(img1, image_points1[i][j], cv::Scalar(0, 255, 0));
            drawCross(img1, proj1[j], cv::Scalar(0, 0, 255));
        }

        // ===== CONCAT =====
        cv::Mat combined;
        cv::hconcat(img0, img1, combined);

        // ===== ZOOM (inchangé) =====
        image_original = combined.clone();
        zoom = 1.0;

        cv::namedWindow("Verification calibration", cv::WINDOW_NORMAL);
        cv::setMouseCallback("Verification calibration", onMouse);

        while (true)
        {
            cv::Mat display;

            int w = image_original.cols;
            int h = image_original.rows;

            int new_w = w / zoom;
            int new_h = h / zoom;

            int x = mouse_x - new_w / 2;
            int y = mouse_y - new_h / 2;

            x = std::max(0, std::min(x, w - new_w));
            y = std::max(0, std::min(y, h - new_h));

            cv::Rect roi(x, y, new_w, new_h);
            cv::Mat cropped = image_original(roi);

            cv::resize(cropped, display, cv::Size(w, h));

            cv::imshow("Verification calibration", display);

            int key = cv::waitKey(30);

            if (key == 27) return; // ESC
            if (key == 'n') break; // next
        }

        cv::destroyAllWindows();
    }
}


// =======================
// MAIN WRAPPER
// =======================
void verification_calibration()
{
    int nb_cam = 2;
    verif_projection(nb_cam);
}