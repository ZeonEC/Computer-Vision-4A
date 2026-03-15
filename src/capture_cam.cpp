#include <iostream>          // bibliothèque pour afficher des messages dans le terminal
#include <opencv2/opencv.hpp> // bibliothèque OpenCV principale (caméra, images, affichage)

#include "capture_cam.hpp"   // header contenant la déclaration de la fonction capture_cam


// ============================================================================
// Fonction : capture_cam
// Rôle : capturer des images depuis la webcam pour la calibration
// Entrée : taille_image → résolution souhaitée de l'image capturée
// Sortie : nombre d'images sauvegardées pour la calibration
// ============================================================================

int capture_cam(cv::Size taille_image)
{

    cv::Size taille_image(600, 600);

    // ouverture de la webcam (index 0 = première caméra du PC)
    // CAP_DSHOW = backend DirectShow (plus stable sous Windows)
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    // vérifie si la caméra s'est correctement ouverte
    if (!cap.isOpened())
    {
        std::cerr << "Erreur ouverture camera\n";
        return 0;
    }

    // configuration de la résolution de capture
    cap.set(cv::CAP_PROP_FRAME_WIDTH, taille_image.width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, taille_image.height);

    // matrice OpenCV qui contiendra chaque image capturée
    cv::Mat frame;

    // compteur du nombre d'images de calibration enregistrées
    int nb_calib = 0;

    // boucle infinie pour lire la webcam en continu
    while (true)
    {
        // capture une image depuis la webcam
        cap >> frame;

        // si l'image est vide (problème caméra) on arrête
        if (frame.empty())
            break;

        // affiche l'image dans une fenêtre
        cv::imshow("Webcam", frame);

        // attend une touche clavier pendant 1 ms
        int key = cv::waitKey(1);

        // si la touche ESC est pressée → on quitte la capture
        if (key == 27)
            break;

        // si la touche 's' ou 'S' est pressée → sauvegarder l'image
        if (key == 's' || key == 'S')
        {
            // incrémente le nombre d'images de calibration
            nb_calib++;

            // construction du nom du fichier image
            std::string filename =
                "../../../calibration_images/calib" +
                std::to_string(nb_calib) + ".png";

            // sauvegarde l'image dans le dossier calibration_images
            cv::imwrite(filename, frame);

            // message dans le terminal
            std::cout << "Image sauvegardee : " << filename << std::endl;
        }
    }

    // libération de la webcam
    cap.release();

    // fermeture de toutes les fenêtres OpenCV
    cv::destroyAllWindows();

    // retourne le nombre total d'images sauvegardées
    return nb_calib;
}