#include "graphics.hpp"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace genv;

const int XX=400;
const int YY=300;

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

struct Vizmagassag
{
    int x;
    int y;
    int m;

    Vizmagassag()
    {
        x = -1;
        y = -1;
        m = 0;
    }

    Vizmagassag(int _x, int _y, int _m)
    {
        x = _x;
        y = _y;
        m = _m;
    }
};

class TerkepKezelo
{
private:
    int XX;
    int YY;
    vector<vector<int>> terkep;
    vector<vector<int>> sziget;
    vector<Koord> utacska;
    vector<vector<int>> vizm;
    vector<Vizmagassag> vizmagassag;

public:
    TerkepKezelo()
    {
        beolvas();
        gout.open(XX,YY);
        kirajzol();
        sziget = vector<vector<int>>(YY,vector<int>(XX,0));
    }

    void beolvas()
    {
        string fnev;
        ifstream temp;

        cout << "Adj meg egy fajlnevet:" << endl;
        cin >> fnev;
        temp.open(fnev);
        while(!temp.good())
        {
            temp.close();
            cout << "Hibas fajlnev!" << endl << "Adj meg egy uj fajlnevet:" << endl;
            cin >> fnev;
            temp.open(fnev);
        }

        temp >> XX;
        temp >> YY;

        terkep = vector<vector<int>>(YY,vector<int>(XX,0));
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                temp >> terkep[j][i];
                Vizmagassag v(i, j, terkep[j][i]);
                vizmagassag.push_back(v);
            }
        }
        cout << "Sikeres beolvasas!" << endl;
    }

    void kirajzol()
    {
        for(Vizmagassag v: vizmagassag)
        {
            int magassag = v.m;
            if(magassag>0)
                gout << move_to(v.x,v.y) << color(0,max(0,min(255,120+magassag)),0) << dot;
            else
                gout << move_to(v.x,v.y) << color(0,0,max(0,min(255,220+magassag))) << dot;
        }
    }

    bool ut_keres(int elozo_x, int aktual_x, int elozo_y,  int aktual_y)
    {
        if(terkep[aktual_y][aktual_x]<=0)
            return false;

        if(terkep[elozo_y][elozo_x]<=0)
            return false;

        for(Vizmagassag v: vizmagassag)
            terkep[v.y][v.x] = v.m;

        sziget = vector<vector<int>>(YY,vector<int>(XX,0));
        sziget[aktual_y][aktual_x]=1;

        vector<Koord> uj_helyek;
        Koord k0(aktual_x, aktual_y);
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
                            Koord uj_k(i,j);
                            sziget[j][i]=1;
                            nagyon_uj_helyek.push_back(uj_k);
                            if(i == elozo_x && j == elozo_y)
                            {
                                cout << "Itt van egy jobbara artalmatlan ut.\n";
                                return true;
                            }
                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }

    cout << "Itt sajnos nincs ut, szoval viszlat es kosz a halakat!\n";
    return false;
    }

    void ut_rajzol(int elozo_x, int aktual_x, int elozo_y,  int aktual_y)
    {
        bool kilep = false;
        int i = elozo_x;
        int j = elozo_y;
        int ut_hossz;
        utacska.clear();
        Koord k(i,j);
        utacska.push_back(k);

        while(!kilep)
        {
            if (i<XX-1 && i < aktual_x && sziget[j][i+1] == 1 && i < 400)
            {
                i += 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utacska.push_back(k);
            }
            else if(j<YY-1 && j < aktual_y && sziget[j+1][i] == 1 && j<300)
            {
                j += 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utacska.push_back(k);
            }
            else if(i>1 && i > aktual_x && sziget[j][i-1] == 1)
            {
                i -= 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utacska.push_back(k);
            }
            else if(j>1 && j > aktual_y && sziget[j-1][i] == 1)
            {
                j -= 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utacska.push_back(k);
            }
            else if(i>1 && j>1 && sziget[j-1][i-1] == 1)
            {
                j -=1;
                i -=1;
                sziget[j][i]=0;
                Koord k(i,j);
                utacska.push_back(k);
            }
            else if(j<YY-1 && i<XX-1 && sziget[j+1][i+1] == 1 && j<300 && i < 400)
            {
                j +=1;
                i +=1;
                sziget[j][i]=0;
            }
            else if(j<YY-1 && sziget[j+1][i-1] == 1 && j<300)
            {
                j +=1;
                i -=1;
                sziget[j][i]=0;
                Koord k(i,j);
                utacska.push_back(k);
            }
            else if(j>1 && i<XX-1 && sziget[j-1][i+1] == 1 && i < 400)
            {
                j -=1;
                i +=1;
                sziget[j][i]=0;
                Koord k(i,j);
                utacska.push_back(k);
            }
            else
            {
                utacska.pop_back();
                ut_hossz = utacska.size()-1;
                j = utacska[ut_hossz].y;
                i = utacska[ut_hossz].x;
            }
            if(j==aktual_y && i == aktual_x)
                kilep = true;
        }

        cout << "A megteendo ut hossza: " << utacska.size() << " egyseg." << endl;
        cout << "Kereshetsz uj utakat!\n"<< endl;

        for(Koord lepes: utacska)
            gout << move_to(lepes.x, lepes.y) << color(0,0,0) << dot;
    }
};


int main()
{
    TerkepKezelo terkep;
    gout << refresh;
    int elso_x = 0;
    int elso_y = 0;

    cout << "Az ut kereses menete a kovetkezo: az egerrel eloszor jobb, majd bal klikk a pontok kijelolesehez.\n" << endl;

    event ev;

    while(gin >> ev && ev.keycode != key_escape)
    {
        if(ev.type == ev_mouse)
        {
            if(ev.button == btn_right)
            {
                elso_x = ev.pos_x;
                elso_y = ev.pos_y;
                terkep.kirajzol();
                gout << refresh;
            }
            if(ev.button == btn_left)
                {
                    terkep.kirajzol();
                    bool rajzol = terkep.ut_keres(elso_x, ev.pos_x, elso_y, ev.pos_y);
                    if(rajzol)
                        terkep.ut_rajzol(elso_x, ev.pos_x, elso_y, ev.pos_y);
                }
                gout << refresh;
        }
    }
    return 0;
};
