#include "graphics.hpp"
#include <vector>
#include <iostream>
#include <fstream>

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

    void frissit()
    {
        gout << refresh;
    }
};

int main()
{
    TerkepKezelo terkep;
    terkep.frissit();

    event ev;

    while(gin >> ev)
    {

    }
    return 0;
}
