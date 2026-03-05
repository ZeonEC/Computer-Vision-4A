// src/main.cpp
#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Sur Windows + Visual Studio, CAP_DSHOW est souvent le plus stable.
    // Si ça ne marche pas, remplace par cv::CAP_MSMF.
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    if (!cap.isOpened()) {
        std::cerr << "Erreur: impossible d'ouvrir la camera (index 0).\n";
        std::cerr << "Essaie cv::CAP_MSMF ou un autre index (1, 2...).\n";
        return 1;
    }

    // Optionnel: forcer une résolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "Erreur: frame vide.\n";
            break;
        }

        cv::imshow("Webcam", frame);

        // ESC pour quitter
        int key = cv::waitKey(10);
        if (key == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}