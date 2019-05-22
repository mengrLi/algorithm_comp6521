#include <iostream>
#include <fstream>
#include <vector>


int minCost(int n, int a, int i, std::vector<int> vector);

using namespace std;


int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
    }
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

    /*
     *
     */
    std::vector<int> cost;// cost of amount, from 0
    ifstream finc(argv[3]);
    for (int j = 0; j < 10001; j++) {
        int amount, c;
        finc >> amount;
        finc >> c;
        cost.push_back(c);
    }
    int dataSetNum;
    fin >> dataSetNum;
    if (dataSetNum == 0) {
        cout << "empty file" << endl;
    }

    for (int i = 0; i < dataSetNum; i++) {
        int N, A, B;
        fin >> N;
        fin >> A;
        fin >> B;
        int min = minCost(N, A, B, cost);
        if(i!=dataSetNum-1)
        {
        fout << min << endl;
        }
        else{
            fout << min;

        }
    }


    return 0;
}

int minCost(int N, int A, int B, std::vector<int> cost) {
    if (B > A * N)
        B = A * N;
    int X = A + B;

    vector<int> row(X + 1);
    vector<vector<int> > L(N+1);
    for (size_t i = 0; i < L.size(); ++i)
        L[i] = row;


    vector<vector<int> > S(N+1);
    for (size_t j = 0; j < S.size(); ++j)
        S[j] = row;

    int x, n, k;
    int min;
    vector<int> minarray;
    vector<int> record;



    if (B == 0) {
        min = N * cost[A];
        vector<int> re;
        for(int i=0;i<N;i++) {
            if (i == N - 1) {
                cout << A << endl;
            } else {
                cout << A << " ";
            }
        }


        return min;

    } else if (N == 1) {
        min = cost[A];
        cout<<A;
        return min;

    } else {
        for(k=0;k<=A+B;k++)
        {
            L[0][k]=100000;
            S[0][k]=100000;

            L[1][k]=cost[k];
            S[1][k]=k;

        }

        for (n = 2; n <=N; ++n) {
            for (k = 0; k <= A + B; ++k) {
                int mincost = 1000000;
                for (x = 0; x <= A + B; ++x) {
                    if (S[n - 1][x] <= A * N) {
                        if (S[n - 1][x] + k >= A * n && S[n - 1][x] + k <= A * n + B) {
                            if (k + S[n - 1][x] <= A * N) {
                                int curcost = cost[k] + L[n - 1][x];
                                if (curcost < mincost) {
                                    L[n][k] = curcost;
                                    mincost = curcost;

                                    S[n][k] = k + S[n - 1][x];
                                    if (n == N && S[n][k] == N * A) {
                                        minarray.push_back(mincost);
                                    }

                                }
                            }

                        }


                    }
                }

            }

        }

        int temp = 100000;
        for (int c = 0; c < minarray.size(); c++) {
            if (minarray[c] < temp && minarray[c] > 0) {
                temp = minarray[c];
                min = minarray[c];
            }
        }

        //todo
        int result=min;
        int sum=N*A;
        for(n=N;n>0;n--){
            for (x = 0; x <= A + B; ++x) {
                if (S[n][x] == sum && L[n][x] == result) {
                    //cout << "x: " << x << endl;
                    record.push_back(x);
                    result = result - cost[x];
                    sum = sum - x;
                    break;
                }
            }
        }

        //cout<<"record size is:"<<record.size()<<endl;
        for (int i=record.size()-1; i >=0; --i) {

            if(i==0)
            {
                cout<<record[i]<<endl;

            } else{
            cout <<record[i]<<" " ;}

        }

        return min;
    }
}



