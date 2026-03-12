#include <iostream>
#include <opencv2/opencv.hpp>

#include "capture_cam.hpp"

int capture_cam(cv::Size taille_image)
{
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    if (!cap.isOpened())
    {
        std::cerr << "Erreur ouverture camera\n";
        return 0;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, taille_image.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, taille_image.height);

    cv::Mat frame;
    int nb_calib = 0;

    while (true)
    {
        cap >> frame;

        if (frame.empty())
            break;

        cv::imshow("Webcam", frame);

        int key = cv::waitKey(1);

        if (key == 27)
            break;

        if (key == 's' || key == 'S')
        {
            nb_calib++;

            std::string filename =
                "../../../calibration_images/calib" +
                std::to_string(nb_calib) + ".png";

            cv::imwrite(filename, frame);

            std::cout << "Image sauvegardee : " << filename << std::endl;
        }
    }

    cap.release();
    cv::destroyAllWindows();

    return nb_calib;
}