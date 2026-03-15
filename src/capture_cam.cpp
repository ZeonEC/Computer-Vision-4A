#include <iostream>
#include <filesystem>

#include "capture_cam.hpp"

int capture_cam(cv::Size taille_image, int& cur_cam) {

    cv::VideoCapture cap(cur_cam, cv::CAP_DSHOW);

    if (!cap.isOpened()) {
        std::cerr << "Erreur: impossible d'ouvrir la camera (index 0).\n";
        std::cerr << "Essaie CAP_MSMF ou un autre index (1,2...).\n";
    }

    //fixer une résolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, taille_image.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, taille_image.height);

    const std::string winName = "Webcam (ESC pour quitter)";
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    // donne au fichier son numéro d'identification pour la calibration
    int nb_calib = 0;

    cv::Mat frame;
    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "Erreur: frame vide.\n";
            break;
        }

        cv::imshow(winName, frame);

        int key = cv::waitKey(1);
        if (key == 27) break; // ESC

        // Sauvegarde de la photo pour la calibration
        if (key == 's' || key == 'S') {
            // numéro de la photo
            nb_calib++;
            // chemin de sauvegarde
            std::string img_path = "../../../calibration_images/";
            std::string filename = img_path + "calib_cam" + std::to_string(cur_cam) + "_" + std::to_string(nb_calib) + ".png";

            if (cv::imwrite(filename, frame)) {
                std::cout << "Image sauvegardée: " << filename << "\n";
            }
            else {
                std::cerr << "Erreur: impossible de sauvegarder l'image.\n";
            }
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return nb_calib;
}