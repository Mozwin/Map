#include "graphics.hpp"
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
using namespace genv;
using namespace std;

color Szint(int m){
    int r=0;
    int g=255*(m>=0);
    int b= 255*(m<=0);
    color e(r , g , b);
    return e;
}

struct Kep {
private:
    vector<vector<int>> kep;
    int XX=0, YY=0;
public:
    Kep (string fname){
        ifstream f(fname);
        if (!f.good()){
            cerr <<" Nincs meg a file " << endl;
            exit(1);
        }
        f>> XX >> YY;
        cout << XX << " " << YY << endl;
        kep= vector<vector<int>> (XX, vector<int>(YY,0));
        for (int y=0; y<YY; y++)
        {
            for(int x=0; x<XX; x++)
            {
                int m;
                f>>m;
                kep [x][y]=m;
            }
        }
    }
    int xSize() {return XX;}
    int ySize() {return YY;}
    void rajzol (int ex,int ey) {
        float zoom=(ex+1)/100;

        for (int x=0; x<XX; x++){
            for (int y=0; y<YY; y++){
                    int sx=float(x)/zoom;
                    int sy=float(x)/zoom;
                gout << move_to(x,y) << Szint(kep[x][y])<< dot; //rgb
            }
        }
        gout << refresh;
    }
    };

    int main()
    {
        Kep kep("terkep.txt");
        gout.open(kep.xSize(), kep.ySize());
        event ev;
        while(gin >> ev && ev.keycode!= key_escape){
            kep.rajzol(ev.pos_x, ev.pos_y);
            gout<<refresh;
        }
        return 0;
    }

