#include <iostream>
#include <opencv2/opencv.hpp>

#include "verification_calibration.hpp"
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
    std::ofstream logfile("../../../calibration_images/results/verification_projection.txt");

    if (!logfile.is_open()) {
        std::cout << "Impossible de creer le fichier log" << std::endl;
        return;
    }

    std::streambuf* cout_buffer = std::cout.rdbuf();
    std::cout.rdbuf(logfile.rdbuf());

    for (int cur_cam = 0; cur_cam < nb_cam; cur_cam++)
    {
        std::string filename =
            "../../../calibration_images/results/calibration_cam"
            + std::to_string(cur_cam) + "_result.xml";

        cv::Mat camera_matrix, dist_coeffs;
        std::vector<cv::Mat> rvecs, tvecs;
        std::vector<double> perViewErrors;
        double RMS;
        cv::Size taille_image;
        cv::Size pattern_size;
        float square_size;
        std::vector<std::vector<cv::Point2f>> image_points;

        get_calibration_from_xml(filename,
            camera_matrix, dist_coeffs, rvecs, tvecs,
            perViewErrors, RMS, taille_image,
            pattern_size, square_size, image_points);

        std::vector<cv::Point3f> obj;
        std::vector<std::vector<cv::Point3f>> object_points;

        for (int i = 0; i < pattern_size.height; i++)
            for (int j = 0; j < pattern_size.width; j++)
                obj.push_back(cv::Point3f(i * square_size, j * square_size, 0.0f));

        for (size_t i = 0; i < image_points.size(); i++)
            object_points.push_back(obj);

        std::vector<cv::Point2f> projected_points;

        for (int i = 0; i < image_points.size(); i++)
        {
            std::string image_filename =
                "../../../calibration_images/calib_cam"
                + std::to_string(cur_cam) + "_"
                + std::to_string(i + 1) + ".png";

            cv::Mat calib_image = cv::imread(image_filename);

            if (calib_image.empty()) {
                std::cout << "Impossible de charger : " << image_filename << std::endl;
                continue;
            }

            cv::projectPoints(object_points[i], rvecs[i], tvecs[i],
                camera_matrix, dist_coeffs, projected_points);

            for (int j = 0; j < image_points[i].size(); j++)
            {
                drawCross(calib_image, image_points[i][j], cv::Scalar(0, 255, 0)); // vert = points detectes
                drawCross(calib_image, projected_points[j], cv::Scalar(0, 0, 255)); // rouge = points projetes
                //drawCross(calib_image, image_points[i][j], cv::Scalar(0, 255, 0)); // vert = points detectes
            }

            // =======================
            // AFFICHAGE AVEC ZOOM
            // =======================
            image_original = calib_image.clone();
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

                if (key == 27) return; // ESC → quitter

                if (key == 'n') break; // passer à l’image suivante
            }




            cv::destroyAllWindows();
        }
    }

    std::cout.rdbuf(cout_buffer);
    logfile.close();
}

// =======================
// MAIN WRAPPER
// =======================
void verification_calibration()
{
    int nb_cam = 1;
    verif_projection(nb_cam);
}