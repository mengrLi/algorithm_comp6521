//
// Created by mengran li on 2019-03-07.
//

#ifndef SEG_IMAGESEGMENTATION_H
#define SEG_IMAGESEGMENTATION_H
#include <vector>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;


bool BFS(Vertex *g, int n, vector<int> s, int sNum, vector<int> t, int tNum);
void maxFlow(Vertex *g, int n, vector<int> s, int sNum, vector<int> t, int tNum);
//void colorGraph(Mat& in_image, Vertex *g);
void updateFlow(Vertex *g,int v, int u,double path_flow,int t_index);
Mat colorReset(Mat& out_image,Vertex *g);

#endif //SEG_IMAGESEGMENTATION_H
