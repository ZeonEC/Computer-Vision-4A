#pragma once

#include <opencv2/opencv.hpp>

// fonction qui capture des images depuis la webcam
// retourne le nombre d'images sauvegardées
int capture_cam(cv::Size taille_image);