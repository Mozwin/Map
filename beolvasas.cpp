#include <iostream>
#include <fstream>
#include <vector>
#include "graphics.hpp"

using namespace std;

const int XX=800;
const int YY=800;

void beolvas(vector<vector<int> > &t){
    ifstream fin("terkep.txt");
    if(!fin.good()){
        cerr << "Nincs meg a bemeneti fajl!\n";
        exit(1);
    }
}

int main(){

    vector<vector<int> > terkep;
    beolvas(terkep);
    gout.open(X,Y);
   // kirajzol(gout,terkep);
    event ev;
    while(gin >> ev && ev.keycode!=key_escape)
    {

    }
    return 0;
}
