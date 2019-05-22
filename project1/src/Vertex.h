//
// Created by mengran li on 2019-03-06.
//

#ifndef SEG_VERTEX_H
#define SEG_VERTEX_H
#include <vector>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

class Vertex {

public:
    Vertex();
    int adj[4];
    double adjW[4];                 //0:up, 1:right, 2:down, 3:left

    int color;
    int d;
    int pi;
    int index;

};
#endif //SEG_VERTEX_H
