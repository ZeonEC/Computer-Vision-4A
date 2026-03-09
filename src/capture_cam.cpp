#include <iostream>
#include <filesystem>

#include "capture_cam.h"

void capture_cam() {

    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    if (!cap.isOpened()) {
        std::cerr << "Erreur: impossible d'ouvrir la camera (index 0).\n";
        std::cerr << "Essaie CAP_MSMF ou un autre index (1,2...).\n";
     }

    //fixer une résolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 600);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 600);

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
            std::string filename = img_path + "calib" + std::to_string(nb_calib) + ".jpg";
            
            if (cv::imwrite(filename, frame)) {
                std::cout << "Image sauvegardée: " << filename << "\n";
            } else {
                std::cerr << "Erreur: impossible de sauvegarder l'image.\n";
            }
        }
    }

    cap.release();
    cv::destroyAllWindows();
}