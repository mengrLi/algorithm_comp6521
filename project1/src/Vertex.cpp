//
// Created by mengran li on 2019-03-06.
//

#include "Vertex.h"

using namespace std;
using namespace cv;

Vertex::Vertex() {
    index = -1;
    for(int i=0; i<4; ++i)      //default neg position index, no neighbour
        adj[i] = -1;
    for(int i=0; i<4; ++i)      //default 0 weight, no neighbor
        adjW[i] = (double)(0);
    color = 0;                  //0: not visited, white; 1: discovered, gray; 2: explored, black
    d = INT_MAX;
    pi = -1;
}


