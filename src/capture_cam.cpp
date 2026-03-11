#include <iostream>
#include <opencv2/opencv.hpp>

#include "capture_cam.hpp"

void capture_cam()
{
    // création d'un objet VideoCapture permettant d'accéder à la webcam
    // 0 signifie : première caméra du système
    // CAP_DSHOW indique d'utiliser DirectShow (backend caméra sous Windows)
    cv::VideoCapture cap(0, cv::CAP_DSHOW);

    // création d'un objet VideoCapture permettant d'accéder à la webcam
    // 0 signifie : première caméra du système
    // CAP_DSHOW indique d'utiliser DirectShow (backend caméra sous Windows)
    if (!cap.isOpened())
    {
        std::cerr << "Erreur: impossible d'ouvrir la camera.\n";
        return;
    }

    // définition de la résolution de capture
    // on force la caméra à produire des images 600x600
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 600);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 600);

    // nom de la fenêtre d'affichage
    const std::string winName = "Webcam (S = save, ESC = quit)";
    // création de la fenêtre OpenCV
    cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);

    // compteur pour numéroter les images de calibration
    int nb_calib = 0;
    // compteur pour numéroter les images de calibration
    cv::Mat frame;

    // boucle infinie pour lire en continu la webcam
    while (true)
    {
        // lecture d'une image depuis la caméra
        // frame contiendra les pixels de l'image
        if (!cap.read(frame) || frame.empty())
        {
            std::cerr << "Erreur: frame vide.\n";
            break;
        }

        // affichage de l'image dans la fenêtre
        cv::imshow(winName, frame);

        // si la fenêtre est fermée avec la croix
        if (cv::getWindowProperty(winName, cv::WND_PROP_VISIBLE) < 1)
            break;

        // waitKey attend une touche clavier pendant 1 ms
        // il renvoie la valeur ASCII de la touche pressée
        int key = cv::waitKey(1);

        // touche ESC 
        if (key == 27)
            break;

        if (key == 's' || key == 'S')
        {
            // on incrémente le compteur d'image
            nb_calib++;

            std::string img_path = "../../../calibration_images/";

            // construction du nom du fichier
            std::string filename = img_path + "calib" + std::to_string(nb_calib) + ".jpg";
                
            // sauvegarde de l'image dans un fichier
            if (cv::imwrite(filename, frame))
                std::cout << "Image sauvegardee: " << filename << "\n";
            else
                std::cerr << "Erreur sauvegarde\n";
        }
    }
    // sauvegarde de l'image dans un fichier
    cap.release();
    // libération de la caméra
    cv::destroyAllWindows();
}