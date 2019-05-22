#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int TSP(vector<vector<int> > graph, vector<int> city, int index, int curCost, int minCost);

int main(int argc, char *argv[]) {
    ifstream fin(argv[1]);
    if (!fin) {
        cout << "the input file is wrong" << endl;
        return -1;
    }
    ofstream fout(argv[2]);
    if (!fout) {
        cout << "the output file is wrong" << endl;
        return -1;
    }
    int numOfGraph;
    fin >> numOfGraph;
    if (numOfGraph == 0) {
        cout << "empty file" << endl;
    }
    for (int i = 0; i < numOfGraph; i++) {
        int numOfVertex, numberOfEdge;
        fin >> numOfVertex;
        fin >> numberOfEdge;
        vector<int> vertices(numOfVertex);
        vector<vector<int> > graph(numOfVertex, vertices);
        for (int j = 0; j < numberOfEdge; ++j) {
            int u, v, weight;
            fin >> u;
            fin >> v;
            fin >> weight;
            graph[u][v] = weight;
            graph[v][u] = weight;
        }
        vector<int> city(numOfVertex);
        for (int j = 0; j < numOfVertex; ++j)
            city[j] = j;
        int minCost = TSP(graph, city, 0, 0, 1000000);
        fout << minCost << endl;
    }
    return 0;
}

int TSP(vector<vector<int> > graph, vector<int> city, int index, int curCost, int minCost) {
    int n = city.size();
    if (index < n - 1) { // select next city
        for (int i = index + 1; i < n; ++i) {
            int t = city[index + 1];
            city[index + 1] = city[i];
            city[i] = t;
            int tmpCost = curCost + graph[city[index]][city[index + 1]];
            if (tmpCost < minCost) {
                minCost = min(minCost, TSP(graph, city, index + 1, tmpCost, minCost));
            }
            int temp = city[index + 1];
            city[index + 1] = city[i];
            city[i] = temp;
        }
    } else {//perform the last city
        minCost = min(minCost, curCost + graph[city[0]][city[n - 1]]);
    }
    return minCost;
}