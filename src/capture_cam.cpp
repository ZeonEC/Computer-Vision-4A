#include <iostream>      // pour afficher des messages dans la console
#include <filesystem>    // pour manipuler des chemins de fichiers

#include "capture_cam.hpp" // fichier header de la fonction

// fonction qui capture des images avec la webcam
// elle retourne un entier = nombre d'images sauvegardées pour la calibration
int capture_cam(cv::Size taille_image) {

    // ouvre la webcam (0 = première caméra du PC)
    // CAP_DSHOW = backend DirectShow sous Windows (plus stable que le défaut)
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    // vérifie si la caméra s'est bien ouverte
    if (!cap.isOpened()) {
        std::cerr << "Erreur: impossible d'ouvrir la camera (index 0).\n";
        std::cerr << "Essaie CAP_MSMF ou un autre index (1,2...).\n";
    }

    // fixe la résolution de l'image capturée
    cap.set(cv::CAP_PROP_FRAME_WIDTH, taille_image.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, taille_image.height);

    // nom de la fenêtre d'affichage
    const std::string winName = "Webcam (ESC pour quitter)";

    // création de la fenêtre OpenCV
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    // compteur du nombre d'images prises pour la calibration
    int nb_calib = 0;

    // matrice qui contiendra chaque image capturée
    cv::Mat frame;

    // boucle infinie pour lire la webcam en continu
    while (true) {

        // capture une image depuis la webcam
        // read() remplit la matrice frame
        if (!cap.read(frame) || frame.empty()) {
            std::cerr << "Erreur: frame vide.\n";
            break;
        }

        // affiche l'image dans la fenêtre
        cv::imshow(winName, frame);

        // attend une touche clavier pendant 1 ms
        int key = cv::waitKey(1);

        // si ESC est pressé → quitter le programme
        if (key == 27) break;

        // si la touche 's' ou 'S' est pressée → sauvegarder l'image
        if (key == 's' || key == 'S') {

            // incrémente le compteur d'image
            nb_calib++;

            // chemin du dossier où enregistrer les images
            std::string img_path = "../../../calibration_images/";

            // nom du fichier (calib1.png, calib2.png, etc.)
            std::string filename = img_path + "calib" + std::to_string(nb_calib) + ".png";

            // sauvegarde l'image
            if (cv::imwrite(filename, frame)) {
                std::cout << "Image sauvegardée: " << filename << "\n";
            }
            else {
                std::cerr << "Erreur: impossible de sauvegarder l'image.\n";
            }
        }
    }

    // libère la webcam
    cap.release();

    // ferme toutes les fenêtres OpenCV
    cv::destroyAllWindows();

    // retourne le nombre total d'images capturées
    return nb_calib;
}