#ifndef SEAMCARVINGCOMP665156
#define SEAMCARVINGCOMP665156

#include <opencv2/opencv.hpp>

// the function you need to implement - by defaiult it calls seam_carving_trivial
bool seam_carving(cv::Mat& in_image, int new_width, int new_height, cv::Mat& out_image);

bool seam_carving_trivial(cv::Mat& in_image, int new_width, int new_height, cv::Mat& out_image);

void compute_gradient(cv::Mat& in_image, cv::Mat& grad, cv::Mat& gray);

void compute_hsum(cv::Mat &grad, int *h_energy);
void compute_vsum(cv::Mat &grad, int *v_energy);

void find_hmin_value(int *h_energy, int *horizontal_min_tract, int rows, int cols);
void find_vmin_value(int *v_energy, int *vertical_min_tract, int rows, int cols);

void reduce_horizontal_seam_trivial(cv::Mat &in_image, int *horizontal_min_tract, cv::Mat &out_image);
void reduce_vertical_seam_trivial(cv::Mat &in_image, int *vertical_min_tract, cv::Mat &out_image);

void find_hseam(int *h_energy, int *horizontal_min_tract, int min, int min_i, int rows, int cols);

void find_vseam(int *v_energy, int *vertical_min_tract, int min, int j, int rows, int cols);


#endif
