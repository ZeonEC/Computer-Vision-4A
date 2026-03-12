#include <iostream>

#include "menu.hpp"
#include "capture_cam.hpp"
#include "calibration.hpp"
#include "charger_calibration.hpp"

// ============================================================================
// Fonction : menu
// Rôle : afficher le menu principal du programme et gérer les choix utilisateur
// Ce menu permet :
//   1 → prendre des photos pour la calibration
//   2 → effectuer la calibration de la caméra
//   3 → charger une calibration existante
//   0 → quitter le programme
// ============================================================================

void menu()
{
    int choix; // variable qui stocke le choix de l'utilisateur

    // taille des images capturées par la webcam
    // (600 x 600 pixels dans ce projet)
    cv::Size taille_image(600, 600);

    // boucle infinie : le menu reste affiché tant que l'utilisateur ne quitte pas
    while (true)
    {
        // affichage des options du menu
        std::cout << "\n===== MENU =====\n";
        std::cout << "1 : Prendre photos\n";
        std::cout << "2 : Calibration\n";
        std::cout << "3 : Charger calibration\n";
        std::cout << "0 : Quitter\n";

        // lecture du choix utilisateur dans le terminal
        std::cin >> choix;

        // --------------------------------------------------------------------
        // OPTION 1 : capture d'images pour la calibration
        // ouvre la webcam et permet de sauvegarder des images du damier
        // --------------------------------------------------------------------
        if (choix == 1)
        {
            capture_cam(taille_image);
        }

        // --------------------------------------------------------------------
        // OPTION 2 : lancer la calibration de la caméra
        // étapes :
        //   1. demander combien d'images utiliser
        //   2. détecter les coins du damier dans chaque image
        //   3. afficher les points détectés
        //   4. lancer le calcul de calibration
        // --------------------------------------------------------------------
        else if (choix == 2)
        {
            int nb;

            // demande à l'utilisateur combien d'images de calibration utiliser
            std::cout << "Nombre d'images calibration : ";
            std::cin >> nb;

            // récupération des coins du damier dans les images
            auto pts = get_grid_points(nb);

            // affichage des coordonnées des coins détectés
            show_grid_points(pts);

            // lancement du calcul de calibration
            calibration(pts, taille_image);
        }

        // --------------------------------------------------------------------
        // OPTION 3 : charger une calibration déjà calculée depuis un fichier XML
        // --------------------------------------------------------------------
        else if (choix == 3)
        {
            charger_calibration();
        }

        // --------------------------------------------------------------------
        // OPTION 0 : quitter le programme
        // --------------------------------------------------------------------
        else if (choix == 0)
        {
            break;
        }
    }
}