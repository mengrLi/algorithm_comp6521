//
// Created by mengran li on 2019-03-07.
//
#include "Vertex.h"
#include "ImageSegmentation.h"


using namespace std;
using namespace cv;

Mat colorReset(Mat& out_image,Vertex *g){
    int width = out_image.cols;
    int height = out_image.rows;
    int n = width*height;

    for(int i=0; i<n; ++i){
        int x, y;
        Vec3b pixel;
        if(g[i].color == 2){
            x = i%width;
            y = i/width;
            pixel[0] = 255;
            pixel[1] = 255;
            pixel[2] = 255;
        } else{
            x = i%width;
            y = i/width;
            pixel[0] = 0;
            pixel[1] = 0;
            pixel[2] = 0;
        }
        out_image.at<Vec3b>(y, x) = pixel;
    }
    return out_image;
}

bool BFS(Vertex *g, int n, vector<int> s, int sNum, vector<int> t, int tNum){
    //reset the state of each vNode in rGraph
   // bool * visited = new bool[s];
    for(int i=0; i<n; ++i){
        g[i].color = 0; //white
        g[i].d = INT_MAX;
        g[i].pi = -1;
    }

    list <int> queue;
    for(int i=0; i<sNum; ++i){
        int index = s[i];
        g[index].color = 1;            
        g[index].d = 0;                
        queue.push_back(index);
    }
    while(!queue.empty()){
        int index = queue.front();
        Vertex u = g[index];
        queue.pop_front();

        for(int i=0; i<4; ++i){
            if(u.adj[i] > 0 && u.adjW[i] > 0){
                int v = u.adj[i];
                if(g[v].color == 0){
                    g[v].pi = u.index;
                    g[v].d = u.d+1;
                    g[v].color = 1;    
                    queue.push_back(v);
                }
            }
        }
        g[index].color = 2;          
    }
    for(int i=0; i<tNum; ++i){
        if(g[t[i]].color == 2){
            return true;
        }
    }
    return false;
}

void maxFlow(Vertex *g, int n, vector<int> s, int sNum, vector<int> t, int tNum){
    int count = 0;
    while(BFS(g, n, s, sNum, t, tNum))
    {
        count++;
        int index = -1;
        for(int i=0; i<tNum; ++i){
            if(g[t[i]].color == 2){
                index = t[i];
                break;
            }
        }
        if(index > -1){
            double path_flow = DBL_MAX;
            int u, v;                           
            v = index;
            u = g[v].pi;
            double edge = 0;

            while(u > -1){
                for(int i=0; i<4; ++i){         
                    if(v == g[u].adj[i]){
                        edge = g[u].adjW[i];
                        break;
                    }
                }
                path_flow = std::min(path_flow, edge);
                v = u;
                u = g[v].pi;
            }
            updateFlow(g,v,u,path_flow,index);
        }
    }
    BFS(g, n, s, sNum, t, tNum);
}

void updateFlow(Vertex *g,int v, int u,double path_flow,int t_index) {
    v = t_index;
    u = g[v].pi;
    while(u > -1){
        //u to v
        for(int i=0; i<4; ++i){          
            if(v == g[u].adj[i]){
                g[u].adjW[i] -= path_flow;
                break;
            }
        }

        //v to u
        for(int i=0; i<4; ++i){          
            if(u == g[v].adj[i]){
                g[v].adjW[i] += path_flow;
                break;
            }
        }
        v = u;
        u = g[v].pi;
    }

}

