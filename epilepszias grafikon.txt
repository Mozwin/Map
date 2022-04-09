#include "graphics.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace genv;
using namespace std;

struct Koordinata
{
    int x;
    int y;

    Koordinata()
    {
        x = -1;
        y = -1;
    }
    Koordinata(int _x, int _y) : x(_x), y(_y)
    {

    }
};

class Terkep
{
    int X,Y;
    vector<vector<int>> ter;
    vector<vector<int>> sziget;
    vector<vector<int>> szigetek;
public:

    Terkep()
    {
        beolvas("terkep.txt");
        sziget = vector<vector<int>>(X, vector<int>(Y,0));
        szigetek = vector<vector<int>>(X, vector<int>(Y,0));
    }

    void beolvas(string fn)
    {

        ifstream t(fn);
        if(!t.good())
        {
            cout << "Nem sikerult a beolvasas" << endl;
            return;
        }else
        {
            t >> X >> Y;
            ter = vector<vector<int>>(X, vector<int>(Y,0));
            for(int j = 0;j < Y;j++)
            {
                for(int i = 0; i < X; i++)
                {
                    t >> ter[i][j];
                }

            }
        }
    }
    int getX()
    {
        return X;
    }
    int getY()
    {
        return Y;
    }

    void kirajzol()
    {
        for(int j = 0;j < Y;j++)
        {
            for(int i = 0; i < X; i++)
            {
                int magassag = ter[i][j];
                if(magassag > 0)
                {
                    gout << move_to(i,j) << color(0,255,0) << dot;
                }
                else
                {
                    gout << move_to(i,j) << color(0,0,255) << dot;
                }
            }

        }
    }


    void magasagkirajzol(int x,int y)
    {

        gout<<color(0, 0, 0)<<move_to(0, 300)<<box(400, 400);
                    gout<<color(220, 220, 0);
                    for (int k=1; k<=400; k++)
                    {
                          gout<<move_to(k, 400-ter[y][k])<<line_to(k+1, 400-ter[y][k+1]);


                    }
                    gout<<refresh;

    }
};



int main()
{

    Terkep terkep;
    gout.open(terkep.getX()+200,terkep.getY()+200);
    event ev;
    gin.timer(40);
    while(gin >> ev && ev.keycode != key_escape)
    {
        gout << move_to(0,0) << color(0,0,0) << box_to(terkep.getX()+199,terkep.getY()+199);
        terkep.kirajzol();

        if(ev.button==0 and ev.pos_y<=399)
                {
                    terkep.magasagkirajzol(ev.pos_x,ev.pos_y);
                }

        gout << refresh;
    }


    return 0;
}
