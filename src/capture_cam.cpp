#include <iostream>
#include <filesystem>

#include <opencv2/opencv.hpp>

#include "capture_cam.hpp"
#include "calibration.hpp"

void capture_cam()
{
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    if (!cap.isOpened())
    {
        std::cerr << "Erreur: impossible d'ouvrir la camera (index 0).\n";
        std::cerr << "Essaie CAP_MSMF ou un autre index (1,2...).\n";
        return;
    }

    // fixer une résolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 600);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 600);

    const std::string winName = "Webcam (S = save, C = calibrate, ECHAP = quit)";
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

        // quitter
        if (key == 27)
            break;

        // sauvegarder image
        if (key == 's' || key == 'S')
        {
            nb_calib++;

            std::string img_path = "../../../calibration_images/";
            std::string filename = img_path + "calib" + std::to_string(nb_calib) + ".jpg";

            if (cv::imwrite(filename, frame))
            {
                std::cout << "Image sauvegardee: " << filename << std::endl;
            }
            else
            {
                std::cerr << "Erreur: impossible de sauvegarder l'image.\n";
            }
        }

        // lancer calibration
        if (key == 'c' || key == 'C')
        {
            std::cout << "Lancement de la calibration...\n";
            calibration();
        }
    }

    cap.release();
    cv::destroyAllWindows();
}