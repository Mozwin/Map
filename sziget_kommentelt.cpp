#include "graphics.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace genv;

struct Koord  //igy nem kell kulon kezelni a pixelek szomszedjait, ezert jo a seged struct
{
    int x;
    int y;

    Koord()
    {
        x = 0;
        y = 0;
    }

    Koord(int _x, int _y)  //konstruktor
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
    vector<vector<int>> sziget;  //nem a terkep hanem egy binaris maszk
    vector<vector<int>> szigetek;

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

    bool sziget_keres(int posx, int posy)   ///Flood Fill
    {
        if(terkep[posy][posx]<=0) return false;  //korábban y szerint olvastunk, a milyen területnél látszik
                                                 // returm false miatt egyből nem mehet tovább, ezért nincs else ág


        sziget = vector<vector<int>>(YY,vector<int>(XX,0)); //deklarált vectort inicializáljuk nullára: meg kell hívni a vektor konstruktorát
        sziget[posy][posx]=1;  //a bekattintott pont színezése

        vector<Koord> uj_helyek;  //első katt is régi, mert nem nézzük hogy ő szomszéd e csak neki nezzük a szomszédjait
        Koord k0(posx, posy); //ha ez nincs akkor push_back(Koord(posx,posy)) - kövi sorban
        uj_helyek.push_back(k0);




/*
        while(regi_pontok.size()>0 || elso_lepes)   //akkor van vége ha nem kerül bele új
                                    //de alapból nulla így sose lépünk bele ezért van az elso_leoes true fenn!!!
        {


        for(const Koord &k: regi_pontok)    //for each loop, range base alapú iteráció, vektor elemein megy végig
                                            // : operátor lemasolja a vektor elemit ezert van & jel, hogy ne atirja, hanem tenyleg masolja
                                            // const azert van ott, hogy ne tudjuk megvaltoztatni


         sziget[k.x][k.y]=1;  //1- kiszinezem
        {
            for(int int j=max(0,k.y-1); j<=min(YY-1,k.y+1); j++)
                {
                    for(int i=max(0,k.x-1); i<=min(XX-1,k.x+1)   //i=k.x-1; i<=k.x+1; i++
                    {
                        if(terkep[j][i]>0 && sziget[j][i]==0)  //csak szarazfoldre menjen
                        {
                            uj_pontok.push_back(Koord(i,j)); //ha meg nem volt berakjuk az uj pontok koze
                        }
                    }
                }
        }
        regi_pontok=uj_pontok; //szinezés ujbol regi lesz, mert a regieket szinezem
        uj_pontok.clear();
*/




        while(uj_helyek.size()>0)
        {
            vector<Koord> nagyon_uj_helyek;

            for(Koord k: uj_helyek)
            {
                for(int j=max(0,k.y-1); j<=min(YY-1,k.y+1); j++)  //k.y-1 = bal felso sarok
                {
                    for(int i=max(0,k.x-1); i<=min(XX-1,k.x+1); i++)
                    {
                        if(terkep[j][i]>0 && !(i==k.x && j==k.y) && sziget[j][i]==0)
                        {
                            Koord k1(i,j);
                            sziget[j][i]=1;
                            nagyon_uj_helyek.push_back(k1);
                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }

        return true;
    }


    void sziget_rajzol()   //terkep rajzolo utan kelljen meghivni es csak szinez
    {
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                if(sziget[j][i] == 1) //sziget e
                {
                    gout << color(255,0,0) << move_to(i,j) << dot;
                }
            }
        }
    }


void minden_sziget_keres() ///Connected Components
    {
        szigetek = vector<vector<int>>(YY,vector<int>(XX,0));  //szigetek nullazva
        int temp = 0;  //szin inditva
        for(int posy=0; posy<YY; posy++)
        {
            for(int posx=0; posx<XX; posx++)
            {
                if(terkep[posy][posx]>0 && szigetek[posy][posx]==0)
                {
                    temp++;
                    szigetek[posy][posx] = temp;

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
                                        szigetek[j][i] = temp;
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

 void minden_sziget_rajzol()
    {
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                if(szigetek[j][i] >0)
                {
                    int temp = szigetek[j][i];
                    gout << color((13*temp)%256,(17*temp)%256,0) << move_to(i,j) << dot;  //egy klasszikus szinezos trukk
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
                terkep.sziget_keres(ev.pos_x, ev.pos_y);  //!!!!
                terkep.sziget_rajzol();
                //terkep.minden_sziget_keres();
                //terkep.minden_sziget_rajzol();
                gout << refresh;
            }
        }
    }

    return 0;
}

