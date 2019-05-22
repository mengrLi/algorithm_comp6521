#include "sc.h"

using namespace cv;
using namespace std;


bool seam_carving(Mat& in_image, int new_width, int new_height, Mat& out_image){

    // some sanity checks
    // Check 1 -> new_width <= in_image.cols
    if(new_width>in_image.cols){
        cout<<"Invalid request!!! new_width has to be smaller than the current size!"<<endl;
        return false;
    }
    if(new_height>in_image.rows){
        cout<<"Invalid request!!! new_height has to be smaller than the current size!"<<endl;
        return false;
    }
    
    if(new_width<=0){
        cout<<"Invalid request!!! new_width has to be positive!"<<endl;
        return false;

    }
    
    if(new_height<=0){
        cout<<"Invalid request!!! new_height has to be positive!"<<endl;
        return false;
        
    }

    return seam_carving_trivial(in_image, new_width, new_height, out_image);
}


// seam carves by removing trivial seams
bool seam_carving_trivial(Mat& in_image, int new_width, int new_height, Mat& out_image){

    Mat iimage = in_image.clone();
    Mat oimage = in_image.clone();
    while(iimage.rows!=new_height || iimage.cols!=new_width){
        if(iimage.rows>new_height){
            Mat grad1,gray_image1;
            compute_gradient(iimage, grad1,gray_image1);
            int hrow = grad1.rows;
            int hcol = grad1.cols;
            int* h_energy = new int [hrow * hcol];
            int *horizontal_min_tract = new int[hcol];

            compute_hsum(grad1, h_energy);
            find_hmin_value(h_energy, horizontal_min_tract, hrow, hcol);
            reduce_horizontal_seam_trivial(iimage, horizontal_min_tract, oimage);
            iimage = oimage.clone();
            cout<<iimage.rows<<endl;
        }
        
        if(iimage.cols>new_width){
            Mat grad2,gray_image2;
            compute_gradient(iimage, grad2,gray_image2);
            int vrow = grad2.rows;
            int vcol = grad2.cols;
            int* v_energy = new int [vrow * vcol];
            int* vertical_min_tract = new int[vrow];

            compute_vsum(grad2, v_energy);
            find_vmin_value(v_energy, vertical_min_tract, vrow, vcol);
            reduce_vertical_seam_trivial(iimage, vertical_min_tract, oimage);
            iimage = oimage.clone();
            cout<<iimage.cols<<endl;
        }
    }
    out_image = oimage.clone();

    return true;
}

void compute_gradient(Mat& in_image, Mat& gradiant,Mat& gray_image1){
    Mat src = in_image.clone();
    Mat src_gray;

    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    gray_image1 = src_gray;

    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;


    // Gradient X
    Sobel(src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x);
    // Gradient Y
    Sobel(src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, gradiant);
}

void reduce_horizontal_seam_trivial(Mat &in_image, int *horizontal_min_tract, Mat &out_image) {
    int rows = in_image.rows - 1;
    int cols = in_image.cols;
    out_image = Mat(rows, cols, CV_8UC3);

    for(int j=0; j<cols; ++j){
        int i=0;
        while(i<horizontal_min_tract[j]){
            out_image.at<Vec3b>(i, j) = in_image.at<Vec3b>(i, j);
            i++;
        }
        while(i>=horizontal_min_tract[j] && i<rows){
            out_image.at<Vec3b>(i, j) = in_image.at<Vec3b>(i+1, j);
            i++;
        }
    }
}

void reduce_vertical_seam_trivial(Mat &in_image, int *vertical_min_tract, Mat &out_image){
    int rows = in_image.rows;
    int cols = in_image.cols-1;
    //create an image that is 1 pixel thinner
    out_image = Mat(rows, cols, CV_8UC3);
    for(int i=0; i<rows; ++i){
        int j=0;
        while(j<vertical_min_tract[i]){
            out_image.at<Vec3b>(i, j) = in_image.at<Vec3b>(i, j);
            j++;
        }
        while(j>=vertical_min_tract[i] && j<cols){
            out_image.at<Vec3b>(i, j) = in_image.at<Vec3b>(i, j+1);
            j++;
        }
    }
}

void find_hmin_value(int *h_energy, int *horizontal_min_tract, int rows, int cols){
    //check the last row of the v_grad, and find the min
    //print_v_grad(v_grad, rows, cols);
    int min = h_energy[cols-1];
    int min_i = 0;
    for(int i=0; i<rows; ++i){
        if(h_energy[i*cols+cols-1] < min){
            min = h_energy[i*cols+cols-1];
            min_i = i;
        }
    }
    find_hseam(h_energy,horizontal_min_tract,min,min_i,rows,cols);


}

void find_hseam(int* h_energy, int* horizontal_min_tract, int min, int min_i, int rows, int cols) {
    horizontal_min_tract[cols-1] = min_i;
    for(int j=cols-2; j>=0; --j){
        min = h_energy[(min_i)*cols+j];

        if((min_i<rows-1) && (h_energy[(min_i+1)*cols+j]<min)){
            min = h_energy[(min_i+1)*cols+j];
            min_i = min_i+1;
        }
        if((min_i>=1) && (h_energy[(min_i-1)*cols+j]<min)){
            min = h_energy[(min_i-1)*cols+j];
            min_i = min_i-1;
        }
        horizontal_min_tract[j] = min_i;
    }

}

void find_vmin_value(int *v_energy, int* vertical_min_tract, int rows, int cols){
    int min = v_energy[(rows-1)*cols];
    int min_j = 0;
    for(int j=0; j<cols; ++j){
        if(v_energy[(rows-1)*cols+j] < min){
            min = v_energy[(rows-1)*cols+j];
            min_j = j;
        }
    }
    find_vseam(v_energy,vertical_min_tract,min,min_j,rows,cols);


}

void find_vseam(int* v_energy, int* vertical_min_tract, int min, int min_j, int rows, int cols) {
    vertical_min_tract[rows-1] = min_j;
    for(int i=rows-2; i>=0; --i){
        min = v_energy[i*cols+min_j];
        if((min_j<cols-1) && (v_energy[i*cols+min_j+1]<min)){
            min = v_energy[i*cols+min_j+1];
            min_j = min_j+1;
        }
        if((min_j>=1) && (v_energy[i*cols+min_j-1]<min)){
            min = v_energy[i*cols+min_j-1];
            min_j = min_j-1;
        }
        vertical_min_tract[i] = min_j;
    }

}

void compute_hsum(Mat &grad, int *h_energy){
    int rows = grad.rows;
    int cols = grad.cols;
    for(int i=0;i<rows;i++){
        h_energy[i*cols+0] = grad.at<uchar>(i, 0);
    }
    for(int j=1; j<cols; ++j){
        for(int i=0; i<rows; ++i){
                //check col[j-1]] for of each row
                int min = h_energy[i*cols+(j-1)];
                if((i-1>=0) && (h_energy[(i-1)*cols+j-1]<min)){
                    min = h_energy[(i-1)*cols+j-1];
                }
                if((i+1<rows) && (h_energy[(i+1)*cols+j-1]<min)){
                    min = h_energy[(i+1)*cols+j-1];
                }
                
                h_energy[i*cols+j] = grad.at<uchar>(i, j)+min;

            }
        }

}

void compute_vsum(Mat &grad, int* v_energy){
    int rows = grad.rows;
    int cols = grad.cols;
    for(int j=0;j<cols;j++)
    {
        v_energy[0*cols+j] = grad.at<uchar>(0, j);
    }
    for(int i=1; i<rows; ++i){
        for(int j=0; j<cols; ++j){
                //check row[i-1]]
                int min = v_energy[(i-1)*cols+j];
                if((j-1>=0) && (v_energy[(i-1)*cols+j-1]<min)){
                    min = v_energy[(i-1)*cols+j-1];
                }
                if((j+1<cols) && (v_energy[(i-1)*cols+j+1]<min)){
                    min = v_energy[(i-1)*cols+j+1];
                }
                
                v_energy[i*cols+j] = grad.at<uchar>(i, j)+min;

            }
        }

}



