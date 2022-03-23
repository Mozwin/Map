#include "graphics.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cmath>
using namespace std;
using namespace genv;

void modosit(vector<vector<int > > &t, int x, int y, int k, int dir ) {
    int Y=t.size(), X=t[0].size();
    for (int j=y-k; j<=y+k; j++)
        for (int i=x-k; i<=x+k; i++) {
            float dc = sqrt((i-x)*(i-x)+(j-y)*(j-y)+0.001);
            if (dc < k && i>=0 && j>=0 && i<X && j<Y) {
                int d=60/dc*dir;
                if (d>10)
                    d=20;
                if (d<-10)
                    d=-20;
                t[j][i]+=d;
                if (t[j][i]<-125)
                    t[j][i]=-125;
                if (t[j][i]>125)
                    t[j][i]=125;
            }
        }
}

void terkepkirajz(vector<vector<int > > &t ) {
    for (int j=0; j<t.size(); j++) {
        for (int i=0; i<t[j].size(); i++) {
            int r,g,b;
            int c=127+t[j][i];///2+3*(t[i][j]-t[i][(j+1)%Y])+3*(t[i][j]-t[(i+1)%X][j]);
            g=c;
            if (t[j][i]<0) {
                b=255;//t[i][j];
                g=c;
                r=0;
            } else {
                b=c/2;
            }
            gout << color(r,g,b) << move_to(i,j) << dot;
        }
    }
    gout << refresh;

}

int main(int argc, char *argv[]) {
    stringstream ss;
    if (argc==1)
        ss << "terkep.txt";
    for (int i=1; i<argc; i++)
        ss << argv[i] << " ";
    ss << endl;
    string filename;
    ss >> filename;
    ifstream f(filename.c_str());
    if (!f.good()) {
        cerr << "ERROR: a '" << filename << "' fajl nincs meg." << endl;
        exit(1);
    }
    int X, Y;
    f >> X >> Y;
    gout.open(X,Y);
    vector<vector<int> > t(Y, vector<int>(X,0));
    for (int j=0; j<t.size(); j++) {
        for (int i=0; i<t[j].size(); i++) {
            f >> t[j][i];
        }
    }
    terkepkirajz(t);
    event ev;
    int k=10;
    while(gin >> ev && ev.keycode != key_escape) {
        if (ev.button==btn_wheelup)
            k++;
        if (ev.button==btn_wheeldown)
            k--;
        if (ev.button==btn_left) {
            modosit(t,ev.pos_x, ev.pos_y, k, 1);
            terkepkirajz(t);
        }
        if (ev.button==btn_right) {
            modosit(t,ev.pos_x, ev.pos_y, k, -1);
            terkepkirajz(t);
        }
        if (ev.keycode == 's') {
            string file = "modositott_terkep.txt";
            ofstream out(file);
            out << X << " " << Y << endl;
            for (int j=0; j<t.size(); j++) {
                for (int i=0; i<t[j].size(); i++) {
                    out << t[j][i] << " ";
                }
                out << endl;
            }
            cout << "Terkep mentve '" << file << "' nevvel." << endl;
        }
    }
}
