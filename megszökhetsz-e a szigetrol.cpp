#include "graphics.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace genv;

struct Koord
{
    int x;
    int y;

    Koord()
    {
        x = -1;
        y = -1;
    }

    Koord(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
};

class TerkepKezelo
{
    int XX;
    int YY;
    vector<vector<int>> terkep;
    vector<vector<int>> sziget;
    vector<vector<int>> szigetek;
    bool szokes;

public:

    TerkepKezelo()
    {
        beolvas();
        gout.open(XX,YY);
        kirajzol();
        sziget = vector<vector<int>>(YY,vector<int>(XX,0));
        szigetek = vector<vector<int>>(YY,vector<int>(XX,0));
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

    bool sziget_keres(int posx, int posy) ///Flood Fill
    {
        if(terkep[posy][posx]<=0)
        {
            /// Csak akkor lép a program ebbe az if ágba, ha vízre kattintott a felhasználó
            cout << "Nem sziget, eppen uszol!\n";
            return false;
        }


        sziget = vector<vector<int>>(YY,vector<int>(XX,0));
        sziget[posy][posx]=1;

        vector<Koord> uj_helyek;
        Koord k0(posx, posy);
        uj_helyek.push_back(k0);

        while(uj_helyek.size()>0)
        {
            vector<Koord> nagyon_uj_helyek;
            for(Koord k: uj_helyek)
            {
                for(int j=max(0,k.y-1); j<=min(YY-1,k.y+1); j++)
                {
                    for(int i=max(0,k.x-1); i<=min(XX-1,k.x+1); i++)
                    {
                        if(terkep[j][i]>0 && !(i==k.x && j==k.y) && sziget[j][i]==0)
                        {
                            if(i==0 || i == XX-1 || j == 0 || j ==YY-1)
                                szokes = true; /// amikor sziget hozzáér a képernyõ széléhez
                            Koord k1(i,j);
                            sziget[j][i]=1;
                            nagyon_uj_helyek.push_back(k1);
                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }
        /// a szökés lehetõségétõl függõen a kíratás:
        if (szokes)
            cout << "Megszokhetsz a szigetrol :-)\n";
        else
            cout << "A szigeten ragadtal :-(\n";
        szokes = false;
        return true;
    }

    void minden_sziget_keres() ///Connected Components
    {
        szigetek = vector<vector<int>>(YY,vector<int>(XX,0));
        int ID = 0;
        for(int posy=0; posy<YY; posy++)
        {
            for(int posx=0; posx<XX; posx++)
            {
                if(terkep[posy][posx]>0 && szigetek[posy][posx]==0)
                {
                    ID+=1;
                    szigetek[posy][posx] = ID;

                    vector<Koord> uj_helyek;
                    Koord k0(posx,posy);
                    uj_helyek.push_back(k0);

                    while(uj_helyek.size()!=0)
                    {
                        vector<Koord> nagyon_uj_helyek;
                        for(Koord k: uj_helyek)
                        {
                            for(int j=max(0,k.y-1); j<=min(YY-1,k.y+1); j++)
                            {
                                for(int i=max(0,k.x-1); i<=min(XX-1,k.x+1); i++)
                                {
                                    if(!(i==k.x && j==k.y) && terkep[j][i]>0 && szigetek[j][i]==0)
                                    {
                                        Koord k1(i,j);
                                        nagyon_uj_helyek.push_back(k1);
                                        szigetek[j][i] = ID;
                                    }
                                }
                            }
                        }
                        uj_helyek=nagyon_uj_helyek;
                    }
                }
            }
        }
    }

    void sziget_rajzol()
    {
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                if(sziget[j][i] == 1)
                {
                    gout << color(255,0,0) << move_to(i,j) << dot;
                }
            }
        }
    }

    void minden_sziget_rajzol()
    {
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                if(szigetek[j][i] >0)
                {
                    int ID = szigetek[j][i];
                    gout << color((13*ID)%256,(17*ID)%256,0) << move_to(i,j) << dot;
                }
            }
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
                terkep.kirajzol();
                terkep.sziget_keres(ev.pos_x, ev.pos_y);
                terkep.sziget_rajzol();
                gout << refresh;
            }
        }
        if(ev.type == ev_key)
        {
            if(ev.keycode == key_space)
            {
                terkep.kirajzol();
                terkep.minden_sziget_keres();
                terkep.minden_sziget_rajzol();
                gout << refresh;
            }
        }
    }
    return 0;
}
