#include <fstream>
#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

const int XX=400;
const int YY=300;

int main() {
    int X=XX;
    int Y=YY;
    string filename = "terkep.txt";
    cout << "Terkep generalas...";
    ofstream f(filename);
    vector<vector<int> > t(Y,vector<int>(X,0));
    int K=7;
    for (int l=0; l<K; l++) {
        for (int i=0; i<t.size(); i++) {
            for (int j=0; j<t[i].size(); j++) {
                t[i][j]+=rand()%(2<<(K-l))-(2<<(K+1-l));
            }
        }
        for (int k=0; k<K-l; k++)
            for (int i=0; i<t.size(); i++) {
                for (int j=0; j<t[i].size(); j++) {
                    t[i][j]=(4*t[i][j]+t[i][(j+X-1)%X]+t[i][(j+1)%X]+t[(i+Y-1)%Y][j]+t[(i+1)%Y][j])/8;
                }
            }
    }

    int min, max;
    min=max=t[0][0];
    for (int i=0; i<t.size(); i++) {
        for (int j=0; j<t[i].size(); j++) {
            if (t[i][j]<min)
                min=t[i][j];
            if (t[i][j]>max)
                max=t[i][j];
        }
    }
    for (int i=0; i<t.size(); i++) {
        for (int j=0; j<t[i].size(); j++) {
            t[i][j]=(t[i][j]-min)/float(max-min)*250-125;
        }
    }


    f << X << " " << Y << endl;
    for (int i=0; i<t.size(); i++) {
        for (int j=0; j<t[i].size(); j++) {
            f << t[i][j] << " ";
        }

        f << endl;
    }
    f.close();
    cout << "\rTerkep elkeszult \"" << filename << "\" nevvel." << endl;
    return 0;
}
