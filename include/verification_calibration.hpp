#pragma once

void verif_projection(int& nb_cam);
void verif_distortion(int nb_to_try, cv::Mat camera_matrix, cv::Mat dist_coeffs, cv::Size& taille_image, int cur_cam);
void drawCross(cv::Mat& image, cv::Point2f pt, cv::Scalar color);
void onMouse(int event, int x, int y, int flags, void*);