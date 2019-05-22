#include <opencv2/opencv.hpp>
#include <iostream>
#include <limits.h>
#include <queue>
#include <cmath>
#include <algorithm>
#include <vector>
#include "Vertex.h"
#include "ImageSegmentation.h"

//./seg simple.png config.txt result.png
using namespace cv;
using namespace std;

Vertex *initGraph(int width, int height);                            
void computeGradient(Mat& in_image, Mat& grad, Mat& gray_image);
void computeCapacity(Mat &grad, Mat &gray_image, Vertex *pGraph);


Vertex *initGraph(int width, int height){
    int n = width*height;
    Vertex *pGraph = new Vertex[n];
    Vertex v= Vertex();
    int col, row;
    for(int i=0; i<n; ++i){
        v.index = i;
        col = i%width;
        row = i/width;
        // up
        if(row > 0){
            v.adj[0] = (row-1)*width+(col);
        }
        //right
        if(col < (width-1)){
            v.adj[1] = (row)*width+(col+1);
        }
        //down
        if(row < (height-1)){
            v.adj[2] = (row+1)*width+(col);
        }
        //left
        if(col > 0){
            v.adj[3] = (row)*width+(col-1);
        }
        pGraph[i] = v;
    }
    return pGraph;
}

void computeCapacity(Mat &grad, Mat &gray_image, Vertex *pGraph)
{
    int width = grad.cols;
    int height = grad.rows;
    int k=0;

    //process weight line by line
    for(int i=0; i<height; ++i){
        for(int j=0; j<width; ++j){
            double  g1, w;

            g1 = grad.at<uchar>(i, j);

            //go up capacity
            if(j == 0)
                pGraph[k].adjW[0] = -1;
            else{
                w = abs(127 - g1);
                pGraph[k].adjW[0] = w*w*w;
            }
            //go right capacity
            if(j == width-1)
                pGraph[k].adjW[1] = -1;
            else{
                w = abs(127 - g1);
                pGraph[k].adjW[1] =w*w*w;
            }

            //go down capacity
            if(i == height-1)
                pGraph[k].adjW[2] = -1;
            else{
                w = abs(127 - g1);
                pGraph[k].adjW[2] = w*w*w;
            }

            // go left capacity
            if(j == 0)
                pGraph[k].adjW[3] = -1;
            else{
                w = abs(127 - g1);
                pGraph[k].adjW[3] = w*w*w;
            }
            k++;
        }
    }
}

void computeGradient(Mat& in_image, Mat& grad, Mat& gray_image){
    Mat src = in_image.clone();
    Mat src_gray;

    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    gray_image = src_gray;

    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    // Gradient X
    Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
    // Gradient Y
    Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );


    convertScaleAbs( grad_x, abs_grad_x );
    convertScaleAbs( grad_y, abs_grad_y );
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
}


int main( int argc, char** argv )
{
    if(argc!=4){
        cout<<"Usage: ../seg input_image initialization_file output_mask"<<endl;
        return -1;
    }
 
    Mat in_image;
    in_image = imread(argv[1]/*, CV_LOAD_IMAGE_COLOR*/);
   
    if(!in_image.data)
    {
        cout<<"Could not load input image!!!"<<endl;
        return -1;
    }

    if(in_image.channels()!=3){
        cout<<"Image does not have 3 channels!!! "<<in_image.depth()<<endl;
        return -1;
    }

    ifstream f(argv[2]);
    if(!f){
        cout<<"Could not load initial mask file!!!"<<endl;
        return -1;
    }


    // the output image
    Mat out_image = in_image.clone();
    Mat grad, gray_image;
    int width = in_image.cols;
    int height = in_image.rows;

    computeGradient(in_image, grad, gray_image);
    Vertex *targetGraph= initGraph(width, height);
    computeCapacity(grad, gray_image, targetGraph);

    int n;
    f>>n;               //number of mask pixels

    int sNum =0;
    int tNum =0;
    vector<int>source(n);
    vector<int>sink(n);
    for(int i=0;i<n;++i){
        int x, y, t;    
        f>>x>>y>>t;
        
        if(x<0 || x>=width || y<0 || y>=height){
            cout<<"Invalid pixel mask!"<<endl;
            return -1;
        }

        int index = y*width+x;
        if(t==1){  
            source[sNum] = index;
            sNum++;
        } else {  
            sink[tNum] = index;
            tNum++;
        }
    }
    vector<int>s(sNum);
    vector<int>t(tNum);
    int size=width*height;
    for(int i=0; i<sNum; ++i)
        s[i] = source[i];
    for(int i=0; i<tNum; ++i)
        t[i] = sink[i];


    maxFlow(targetGraph, size, s, sNum, t, tNum);
    out_image = colorReset(out_image,targetGraph);

    // write it on disk
    imwrite( argv[3], out_image);

    // also display them both
    namedWindow( "Original image", WINDOW_AUTOSIZE );
    namedWindow( "Show Marked Pixels", WINDOW_AUTOSIZE );
    imshow( "Original image", in_image );
    imshow( "Show Marked Pixels", out_image );

    waitKey(0);
    return 0;
}
