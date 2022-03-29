#include "graphics.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace genv;

class TerkepKezelo
{
    int XX;
    int YY;
    vector<vector<int>> terkep;
public:

    TerkepKezelo()
    {
        beolvas();
        gout.open(XX,YY);
        magassagot_kirajzol();
    }

    void beolvas()
    {
        string fnev;
        ifstream is;

        cout << "Adjon meg egy fajlnevet:" << endl;
        cin >> fnev;
        is.open(fnev);
        while(!is.good())
        {
            is.close();
            cout << "Hibas fajlnev!" << endl << "Adjon meg egy uj fajlnevet:" << endl;
            cin >> fnev;
            is.open(fnev);
        }

        is >> XX;
        is >> YY;

        terkep = vector<vector<int>>(YY,vector<int>(XX,0));
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                is >> terkep[j][i];
            }
        }
        cout << "Sikeres beolvasas!" << endl;
    }

    void kirajzol()
    {
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                int ertek = terkep[j][i];
                if(ertek>0)
                {
                    gout << move_to(i,j) << color(0,200,0) << dot;
                }
                else
                {
                    gout << move_to(i,j) << color(0,0,200) << dot;
                }
            }
        }
    }

    void magassagot_kirajzol()
    {
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                int magassag = terkep[j][i];
                if(magassag>0)
                {
                    gout << move_to(i,j) << color(0,max(0,min(255,120+magassag)),0) << dot;
                }
                else
                {
                    gout << move_to(i,j) << color(0,0,max(0,min(255,220+magassag))) << dot;
                }
            }
        }
    }

    void milyen_terulet(int posx, int posy)
    {
        cout << "x:"<< posx << " y:" << posy << "\t";
        if(terkep[posy][posx]>0)
        {
             cout << "szarazfold" << endl;
        }
        else
        {
            cout << "viz" << endl;
        }
    }

    void koriv_rajzol(float posx, float posy, float r)
    {
        float gyokketto = 1.414;
        float lepesszam = 2*3.14*r*2;
        for(float alpha=0; alpha<2*3.14; alpha+=2*3.14/lepesszam)
        {
            float pontx = posx+r/gyokketto*(cos(alpha)-sin(alpha));
            float ponty = posy+r/gyokketto*(sin(alpha)+cos(alpha));
            pontx = max(0,min(XX-1,int(pontx)));
            ponty = max(0,min(YY-1,int(ponty)));
            gout << move_to(pontx, ponty) << color(255,0,0) << dot;
        }
    }
};

int main()
{
    TerkepKezelo terkep;
    gout << refresh;

    event ev;

    while(gin >> ev && ev.keycode != key_escape)
    {
        if(ev.type == ev_mouse)
        {
            if(ev.button == btn_left)
            {
                terkep.milyen_terulet(ev.pos_x,ev.pos_y);
            }
            terkep.magassagot_kirajzol();
            terkep.koriv_rajzol(ev.pos_x,ev.pos_y,30);
            gout << refresh;
        }
    }
    return 0;
}
