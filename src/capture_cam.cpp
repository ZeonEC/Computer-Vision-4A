#include <iostream>
#include <opencv2/opencv.hpp>

#include "capture_cam.hpp"

void capture_cam()
{
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    if (!cap.isOpened())
    {
        std::cerr << "Erreur: impossible d'ouvrir la camera.\n";
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 600);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 600);

    const std::string winName = "Webcam (S = save, ESC = quit)";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    int nb_calib = 0;
    cv::Mat frame;

    while (true)
    {
        if (!cap.read(frame) || frame.empty())
        {
            std::cerr << "Erreur: frame vide.\n";
            break;
        }

        cv::imshow(winName, frame);

        int key = cv::waitKey(1);

        if (key == 27)
            break;

        if (key == 's' || key == 'S')
        {
            nb_calib++;

            std::string img_path = "../../../calibration_images/";
            std::string filename = img_path + "calib" + std::to_string(nb_calib) + ".jpg";

            if (cv::imwrite(filename, frame))
                std::cout << "Image sauvegardee: " << filename << "\n";
            else
                std::cerr << "Erreur sauvegarde\n";
        }
    }

    cap.release();
    cv::destroyAllWindows();
}