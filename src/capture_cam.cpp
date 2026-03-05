// webcam_view.cpp
// OpenCV 4.6 - Windows : affiche la webcam (ESC pour quitter)

#include <opencv2/opencv.hpp>
#include <iostream>

#include "capture_cam.h"

void capture_cam() {
    // Backends courants sous Windows : CAP_DSHOW (souvent stable) ou CAP_MSMF
    cv::VideoCapture cap(0, cv::CAP_DSHOW);
    // Si ça ne marche pas, essaie :
    // cv::VideoCapture cap(0, cv::CAP_MSMF);

    if (!cap.isOpened()) {
        std::cerr << "Erreur: impossible d'ouvrir la camera (index 0).\n";
        std::cerr << "Essaie CAP_MSMF ou un autre index (1,2...).\n";
     }

    // Optionnel: fixer une résolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    const std::string winName = "Webcam (ESC pour quitter)";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "Erreur: frame vide.\n";
            break;
        }

        cv::imshow(winName, frame);

        int key = cv::waitKey(1);
        if (key == 27) break; // ESC
    }

    cap.release();
    cv::destroyAllWindows();
}