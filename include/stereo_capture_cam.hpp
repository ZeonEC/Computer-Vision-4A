#ifndef STEREO_CAPTURE_CAM_HPP
#define STEREO_CAPTURE_CAM_HPP

#include <opencv2/opencv.hpp>
#include <vector>

// fonction qui capture des images depuis la webcam
// retourne le nombre d'images sauvegardées
void stereo_capture_cam(cv::Size taille_image, int& nb_cam);

#endif