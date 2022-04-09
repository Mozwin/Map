#include "graphics.hpp"
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <set>

using namespace std;
using namespace genv;

const int XX=400;
const int YY=300;

///A letöltött térkép generáló kódrészlet
void terkep_generalas()
{
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
}
///struct csak a Koordináták tárolására
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
///struct ami eltárolja a magasságot is
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
/// Vízmagasság < operátora, hogy lehessen setet létrehozni a változóval
bool operator<(Vizmagassag v1, Vizmagassag v2)
    {
        if(v1.x<v2.x)
            return true;
        else if(v1.x == v2.x && v1.y<v2.y)
            return true;
        else if(v1.x == v2.x && v1.y == v2.y && v1.m<v2.m)
            return true;
        return false;
    }
/// függvénykezelő class (a fényforrás legenerálásához van csak használva a programban)
class Equation {
private:
    int x1;
    int y1;
    int z1;
    int x2;
    int y2;
    int z2;
    double hossz;
    double vx;
    double vy;
    double vz;

public: /// a konstruktor kiszámol a beadott koordinátákból egy normált irányvektort
    Equation(Vizmagassag k1, Vizmagassag k2) {
        x1 = k1.x;
        y1 = k1.y;
        z1 = k1.m;
        x2 = k2.x;
        y2 = k2.y;
        z2 = k2.m;
        hossz = sqrt((k1.x-k2.x)*(k1.x-k2.x)+(k1.y-k2.y)*(k1.y-k2.y)+(k1.m-k2.m)*(k1.m-k2.m));
        vx = (k1.x-k2.x)/hossz;
        vy = (k1.y-k2.y)/hossz;
        vz = (k1.m-k2.m)/hossz;
    }
/// visszatér a Z koordinátával
    double height(int x, int y) {
        return ((x-x1)/vx)*vz+z1;
    }
/// visszatér az Y koordinátával
    double yCoord(int x) {
        return (vx*x1+vy*y1-vx*x)/vy;
    }
/// visszatér az X koordinátával
    double xCoord(int y) {
        return (vx*x1+vy*y1-vy*y)/vx;
    }
};
///  3D koordináta struct, ami a távolság szerinti < operátort alklamz
struct Dist {
    int x;
    int y;
    int d;

    Dist(int a, int b, int c) {
        x = a;
        y = b;
        d = c;
    }

    bool operator<(const Dist other) const {
        if (this->d < other.d)
            return true;
        if (this->d == other.d && this->x < other.x)
            return true;
        if (this->d == other.d && this->x == other.x && this->y == other.y)
            return true;
        return false;
    }
};
/// több esetre bontva vizsgálja a térképbe ütközést, vagyis az árnyékolást
/// felírja az egyenenes egyenletét, és pontonkét megvizsgálja hogy van-e valami a fény útjába az adott ponttól.
bool test(const Vizmagassag k1, const Vizmagassag k2, const vector<vector<int>> terkep) {
    Equation eq(k1, k2);
    Koord a1(k1.x,k1.y);
    Koord a2(k2.x,k2.y);
    if (eq.height(k1.x,k1.y) <= terkep[k1.x][k1.y])
        return false;
    if (eq.height(k2.x,k2.y) <= terkep[k2.x][k2.y])
        return false;

    double m;

    if (k1.x == k2.x)
    {
        if (k1.y < k2.y)
        {
            for (int i = k1.y + 1; i < k2.y; i++)
            {
                if (eq.height(k1.x,i) <= terkep[k1.x][i])
                    return false;
            }
        }
        else
        {
            for (int i = k1.y - 1; i > k2.y; i--)
            {
                if (eq.height(k1.x,i) <= terkep[k1.x][i])
                    return false;
            }
        }
    }
    else if (k1.y == k2.y)
    {
        if (k1.x < k2.x)
        {
            for (int i = k1.x + 1; i < k2.x; i++)
            {
                if (eq.height(i,k1.y) <= terkep[i][k1.y])
                    return false;
            }
        } else {
            for (int i = k1.x - 1; i > k2.x; i--)
            {
                if (eq.height(i,k1.y) <= terkep[i][k1.y])
                    return false;
            }
        }
    }
    else if (k1.x < k2.x)
    {
        m = (k1.y - k2.y) / (k1.x - k2.x);
        if ( m > 1.0) {
            for (int i = k1.y - 1; i > k2.y; i--)
            {
                double d = eq.xCoord(i);
                double df = floor(d);
                double dc = ceil(d);
                if (df == dc && eq.height(df,i)<terkep[df][i]){
                    return false;
                } else if (eq.height(df,i) < terkep[df][i] || eq.height(dc, i) < terkep[dc][i]) {
                    return false;
                }
            }
        }
        else if (m <= 1.0 && m >= -1.0)
        {
            for (int i = k1.x; i < k2.x; i++) {
                double d = eq.yCoord(i);
                double df = floor(d);
                double dc = ceil(d);
                if (df == dc && eq.height(i, df)<terkep[i][df]){
                    return false;
                } else if (eq.height(i,df) < terkep[i][df] || eq.height(i,dc) < terkep[i][dc]) {
                    return false;
                }
            }
        } else {
            for (int i = k2.y - 1; i > k1.y; i--) {
                double d = eq.xCoord(i);
                double df = floor(d);
                double dc = ceil(d);
                if (df == dc && eq.height(df,i)<terkep[df][i]){
                    return false;
                } else if (eq.height(df,i) < terkep[df][i] || eq.height(dc, i) < terkep[dc][i]) {
                    return false;
                }
            }
        }
    } else {
        m = (k2.y - k1.y) / (k2.x - k1.x);
        if ( m > 1.0) {
            for (int i = k2.y - 1; i > k1.y; i--) {
                double d = eq.xCoord(i);
                double df = floor(d);
                double dc = ceil(d);
                if (df == dc && eq.height(df,i)<terkep[df][i]){
                    return false;
                } else if (eq.height(df,i) < terkep[df][i] || eq.height(dc, i) < terkep[dc][i]) {
                    return false;
                }
            }
        } else if (m <= 1.0 && m >= -1.0) {
            for (int i = k2.x; i < k1.x; i++) {
                double d = eq.yCoord(i);
                double df = floor(d);
                double dc = ceil(d);
                if (df == dc && eq.height(i, df)<terkep[i][df]){
                    return false;
                } else if (eq.height(i,df) < terkep[i][df] || eq.height(i,dc) < terkep[i][dc]) {
                    return false;
                }
            }
        } else {
            for (int i = k1.y - 1; i > k2.y; i--) {
                double d = eq.xCoord(i);
                double df = floor(d);
                double dc = ceil(d);
                if (df == dc && eq.height(df,i)<terkep[df][i]){
                    return false;
                } else if (eq.height(df,i) < terkep[df][i] || eq.height(dc, i) < terkep[dc][i]) {
                    return false;
                }
            }
        }
    }
    return true;
}

///A Térképkezelő class végzi el a térképpel kapcsolatos számításokat, módosításokat, beolvasásokat
class TerkepKezelo
{
    int XX;
    int YY;
    vector<vector<int>> terkep;
    vector<vector<int>> sziget;
    vector<vector<int>> tengerszem;
    vector<vector<int>> vizm;
    vector<Koord> utaszigeten;
    vector<Vizmagassag> vizmagassag;
    vector<Koord> elso_sziget;
    vector<Koord> masodik_sziget;
    vector<vector<int>> tengerbejaras;

public:

    TerkepKezelo()
    {
        beolvas();
        gout.open(XX,YY);
        kirajzol();
        sziget = vector<vector<int>>(YY,vector<int>(XX,0));
        vizm = vector<vector<int>>(YY,vector<int>(XX,0));
    }

    void uj_beolvasas() ///Újra beolvassa a fájlt, hogy eredeti állapotba kerüljenek az adatok, ne kelljen újraindítani a programot.
    {
        string fnev = "terkep.txt";
        ifstream is;

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
                Vizmagassag v(i, j, terkep[j][i]);
                vizmagassag.push_back(v);
            }
        }
        cout << "Sikeres visszaallitas!" << endl;
    }

    void beolvas() /// A generált térkép beolvasása
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
                Vizmagassag v(i, j, terkep[j][i]);
                vizmagassag.push_back(v);
            }
        }
        cout << "Sikeres beolvasas!" << endl;
    }

    void kirajzol() /// Az aktuális térkép kirajzolása
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

    bool ut_keres(int elozo_x, int aktualis_x, int elozo_y,  int aktualis_y) ///Flood Fill, egy szigeten van-e a két kijelölt pont
    {
        if(terkep[aktualis_y][aktualis_x]<=0)
            return false;

        if(terkep[elozo_y][elozo_x]<=0)
            return false;

        for(Vizmagassag v: vizmagassag)
            terkep[v.y][v.x] = v.m;

        sziget = vector<vector<int>>(YY,vector<int>(XX,0));
        sziget[aktualis_y][aktualis_x]=1;

        vector<Koord> uj_helyek;
        Koord k0(aktualis_x, aktualis_y);
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
                            if(i == elozo_x && j == elozo_y) {
                                cout << "Van ut\n";
                                return true; /// megtalálom a keresett pontot, kilépek a függvényből
                            }
                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }
    cout << "Nincs ut\n";
    return false;
    }

    void ut_rajzol(int elozo_x, int aktualis_x, int elozo_y,  int aktualis_y) /// Keresõfával keres egy utat amit elment egy vektorba, majd kirajzolja az utat
    {
        bool kilep = false;
        int i = elozo_x;
        int j = elozo_y;
        int ut_hossz;
        utaszigeten.clear(); /// ne ragadjon bele több út a vektorba
        Koord k(i,j);
        utaszigeten.push_back(k);

        while(!kilep)
            {
            if (i<XX-1 && i < aktualis_x && sziget[j][i+1] == 1 && i < 400)
            {
                i += 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utaszigeten.push_back(k);
            }
            else if(j<YY-1 && j < aktualis_y && sziget[j+1][i] == 1 && j<300)
            {
                j += 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utaszigeten.push_back(k);
            }
            else if(i>1 && i > aktualis_x && sziget[j][i-1] == 1)
            {
                i -= 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utaszigeten.push_back(k);
            }
            else if(j>1 && j > aktualis_y && sziget[j-1][i] == 1)
            {
                j -= 1;
                sziget[j][i]=0;
                Koord k(i,j);
                utaszigeten.push_back(k);
            }
            else if(i>1 && j>1 && sziget[j-1][i-1] == 1)
            {
                j -=1;
                i -=1;
                sziget[j][i]=0;
                Koord k(i,j);
                utaszigeten.push_back(k);
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
                utaszigeten.push_back(k);
            }
            else if(j>1 && i<XX-1 && sziget[j-1][i+1] == 1 && i < 400)
            {
                j -=1;
                i +=1;
                sziget[j][i]=0;
                Koord k(i,j);
                utaszigeten.push_back(k);
            }
            else
            { /// Ha belebotlik egy pixelbe ahonnan nincs hová mennie, akkor kitörli a vektor utolsó elemét, és vissza is lép egyet.
                utaszigeten.pop_back();
                ut_hossz = utaszigeten.size()-1;
                j = utaszigeten[ut_hossz].y;
                i = utaszigeten[ut_hossz].x;
            }

            if(j==aktualis_y && i == aktualis_x)
                kilep = true; /// ha megtalálom az utam másik végét, lépjen ki az útkereső ciklusból
        }
        cout << "Az ut hossza: " << utaszigeten.size() << endl;

        for(Koord lepes: utaszigeten)  /// kirajzolja az utat.
            gout << move_to(lepes.x, lepes.y) << color(0,0,0) << dot;
    }

    void tengerszem_keres(int posx, int posy) /// Flood Fillel megkeresi az adott tengerszemet, és hozzáad a vízmélységhez
    {
        if(terkep[posy][posx]>=0) /// ha szárazföldre kattintok, egyből lépjen ki
            return;

        for(Vizmagassag v: vizmagassag)
            terkep[v.y][v.x] = v.m;

        tengerszem = vector<vector<int>>(YY,vector<int>(XX,0));
        tengerszem[posy][posx]=1;
        int viz = vizm[posy][posx] +1;

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
                        if(terkep[j][i] - vizm[j][i] < viz && !(i==k.x && j==k.y) && tengerszem[j][i]==0)
                        {
                            Koord k1(i,j);
                            tengerszem[j][i]=1;
                            nagyon_uj_helyek.push_back(k1);
                            vizm[j][i] +=1;
                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }

        vizmagassag.clear();  /// a térképhez hozzáveszem a vízmagasságot is, hogy a kirajzol() függvény meghívásakor az új vizet is figyelembe vegye
        for(int j=0; j<YY; j++)
        {
            for(int i=0; i<XX; i++)
            {
                Vizmagassag v2(i, j, terkep[j][i]-vizm[j][i]);
                vizmagassag.push_back(v2);
            }
        }
        cout << "A vizmagassag itt " << viz << endl;
    }

    void sziget_szele_keres(int elozo_x, int aktualis_x, int elozo_y,  int aktualis_y) /// megkeresi az adott szigetek széleit, és rajzol közéjük hidat
    {
        if( (terkep[aktualis_y][aktualis_x]<=0) || (terkep[elozo_y][elozo_x]<=0))
            return;

        sziget = vector<vector<int> >(YY,vector<int>(XX,0));
        sziget[aktualis_y][aktualis_x]=1;

        elso_sziget.clear();
        masodik_sziget.clear();

        vector<Koord> uj_helyek;
        Koord k0(aktualis_x, aktualis_y);
        uj_helyek.push_back(k0);

        while(uj_helyek.size()>0) /// megkeresem az első sziget szélét, és beleteszem a koordinátkat egy vektorba-be
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
                            Koord k1(i,j);
                            sziget[j][i]=1;
                            nagyon_uj_helyek.push_back(k1);
                            if(j<YY-1 && terkep[j+1][i] <= 0)
                                elso_sziget.push_back(k1);
                            else if(j>1 && i<XX && terkep[j-1][i] <= 0)
                                elso_sziget.push_back(k1);
                            else if(i>1 && j>1 && terkep[j-1][i-1] <= 0)
                                elso_sziget.push_back(k1);
                            else if(j>1 && i<XX-1 && terkep[j-1][i+1] <= 0)
                                elso_sziget.push_back(k1);
                            else if(i>1 && terkep[j][i-1] <= 0)
                                elso_sziget.push_back(k1);
                            else if(i>1 && terkep[j][i+1] <= 0)
                                elso_sziget.push_back(k1);
                            else if(j<YY-1 && i<XX-1 && terkep[j+1][i+1] <= 0)
                                elso_sziget.push_back(k1);
                            else if(i>1 && j<YY-1 && terkep[j+1][i-1] <= 0)
                                elso_sziget.push_back(k1);
                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }

        sziget = vector<vector<int>>(YY,vector<int>(XX,0));
        sziget[elozo_y][elozo_x]=1;

        vector<Koord> uj_helyek2;
        Koord k02(elozo_x, elozo_y);
        uj_helyek2.push_back(k02);

        while(uj_helyek2.size()>0) /// megkeresem az második sziget szélét, és beleteszem a koordinátkat egy vektorba-be
        {
            vector<Koord> nagyon_uj_helyek2;
            for(Koord k: uj_helyek2)
            {
                for(int j=max(0,k.y-1); j<=min(YY-1,k.y+1); j++)
                {
                    for(int i=max(0,k.x-1); i<=min(XX-1,k.x+1); i++)
                    {
                        if(terkep[j][i]>0 && !(i==k.x && j==k.y) && sziget[j][i]==0)
                        {
                            Koord k1(i,j);
                            sziget[j][i]=1;
                            nagyon_uj_helyek2.push_back(k1);
                            if(terkep[j+1][i]<= 0 && j<300)
                                masodik_sziget.push_back(k1);
                            else if(terkep[j-1][i]<= 0)
                                masodik_sziget.push_back(k1);
                            else if(terkep[j-1][i-1]<= 0)
                                masodik_sziget.push_back(k1);
                            else if(terkep[j-1][i+1]<= 0 && i < 400)
                                masodik_sziget.push_back(k1);
                            else if(terkep[j][i-1]<= 0)
                                masodik_sziget.push_back(k1);
                            else if(terkep[j][i+1]<= 0 && i < 400)
                                masodik_sziget.push_back(k1);
                            else if(terkep[j+1][i+1]<= 0 && j<300 && i< 400)
                                masodik_sziget.push_back(k1);
                            else if(terkep[j+1][i-1]<= 0 && j < 300)
                                masodik_sziget.push_back(k1);
                        }
                    }
                }
            }
            uj_helyek2 = nagyon_uj_helyek2;
        }
///Számolás a híd megépítéséhez: mi a legrövidebb út?
        Koord mik1, mik2;
        mik1.x = 1;
        mik1.y = 1;
        mik2.x = 398;
        mik2.y  =298;

        for (Koord koo1: elso_sziget)
        {
            for(Koord koo2: masodik_sziget)
            {
                int tav1 = (mik1.x - mik2.x)*(mik1.x - mik2.x)+(mik1.y - mik2.y)*(mik1.y - mik2.y);
                int tav2 = (koo1.x - koo2.x)*(koo1.x - koo2.x)+(koo1.y - koo2.y)*(koo1.y - koo2.y);
                if( tav1 > tav2 )
                {
                    mik1.x = koo1.x;
                    mik1.y = koo1.y;
                    mik2.x = koo2.x;
                    mik2.y = koo2.y;
                }
            }
        }

        gout << move_to(mik1.x, mik1.y) << color(255,255,255) << line_to(mik2.x, mik2.y);
///Számolás: a híd szel át szigetet?
        double meredekseg = (mik1.y - mik2.y)/(mik1.x - mik2.x);
        double b = mik1.y - meredekseg*mik1.x;
        int kisebb = -1;
        int nagyobb = -1;
        if(mik1.x<mik2.x)
        {
            kisebb = mik1.x;
            nagyobb = mik2.x;
        }
        else
        {
            kisebb = mik2.x;
            nagyobb = mik1.x;
        }

        for(int i = kisebb+1; i<nagyobb-1; i++)
        {
            int yy = meredekseg*i + b;
            if(terkep[yy][i]>0)
            {
                cout << "A hid athalad egy szigeten\n";
                return;
            }
        }
        cout << "A hid nem szel at szigetet\n";
    }

    void tengeralattjaro(int posx, int posy)
    {
        if(terkep[posy][posx] > -10)
            return;

        tengerbejaras = vector<vector<int>>(YY,vector<int>(XX,0));
        tengerbejaras[posy][posx]=1;

        vector<Koord> terulet_szele;

        vector<Koord> uj_helyek;
        Koord k0(posx, posy);
        uj_helyek.push_back(k0);

        while(uj_helyek.size()>0) /// megkeresem a tengeralattjáró bejárható területének szélét, és beleteszem a koordinátkat egy vektorba-ba
        {
            vector<Koord> nagyon_uj_helyek;
            for(Koord k: uj_helyek)
            {
                for(int j=max(0,k.y-1); j<=min(YY-1,k.y+1); j++)
                {
                    for(int i=max(0,k.x-1); i<=min(XX-1,k.x+1); i++)
                    {
                        if(terkep[j][i]<=-10 && !(i==k.x && j==k.y) && tengerbejaras[j][i]==0)
                        {
                            gout << move_to(i,j) << color(min((i+j),255),min(j,255),min(i,255)) << dot;
                            Koord k1(i,j);
                            tengerbejaras[j][i]=1;
                            nagyon_uj_helyek.push_back(k1);
                            if(j<YY-1 && terkep[j+1][i] >-10)
                                terulet_szele.push_back(k1);
                            else if(j>1 && terkep[j-1][i] >-10)
                                terulet_szele.push_back(k1);
                            else if(j>1 && i>1 && terkep[j-1][i-1] >-10)
                                terulet_szele.push_back(k1);
                            else if(j>1 && i<XX-1 && terkep[j-1][i+1] >-10)
                                terulet_szele.push_back(k1);
                            else if(i>1 && terkep[j][i-1] >-10)
                                terulet_szele.push_back(k1);
                            else if(i<XX-1 && terkep[j][i+1] >-10)
                                terulet_szele.push_back(k1);
                            else if(i<XX-1 && j<YY-1 && terkep[j+1][i+1] >-10)
                                terulet_szele.push_back(k1);
                            else if(i>1 && j<YY-1 && terkep[j+1][i-1] >-10)
                                terulet_szele.push_back(k1);
                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }

        for(Koord koo: terulet_szele) /// megkeresi a drón által bejárható területet - ez a feltétel még megengedi a "teleportálást"
        {
            for(int i=koo.x-50; i<koo.x+50; i++)
            {
                for(int j=koo.y-50; j<koo.y+50; j++)
                {
                    if(i>1 && j<YY-1 && j>1 && i<XX-1 &&(i-koo.x)*(i-koo.x)+(j-koo.y)*(j-koo.y)<50*50 && terkep[j][i]<=20 && tengerbejaras[j][i]==0)
                        tengerbejaras[j][i]=1;
                }
            }
        }
        dron_ut_kirajzol(posx, posy);
    }

    void dron_ut_kirajzol(int posx, int posy) ///Flood Fillel bejárja a kattintás helyétől kiinduló legnagyobb területet, vagyis kizárja a teleportálást
    {
        vector<vector<int>> sziget;
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
                        if(tengerbejaras[j][i]==1 && !(i==k.x && j==k.y) && sziget[j][i]==0)
                        {
                            Koord k1(i,j);
                            sziget[j][i]=1;
                            nagyon_uj_helyek.push_back(k1);
                            gout << move_to(i,j) << color(min((i+j),255),min(j,255),min(i,255)) << dot;

                        }
                    }
                }
            }
            uj_helyek = nagyon_uj_helyek;
        }
    }

    void fenyforras(int posx, int posy, int _m)
    {
        if(terkep[posy][posx]>=_m)
        {
            cout << "Ne tegyel a foldbe fenyforrast\n";
            return;
        }

        set<Vizmagassag> fenyek;
        fenyek.clear();
        Vizmagassag kiindulas(posx, posy, _m);

        for(int j=posy-25; j<posy+25; j++)
        {
            for(int i=posx-25; i<posx+25; i++)
            {
                if(j<YY && i<XX && i>0 && j>0 && (i-posx)*(i-posx)+(j-posy)*(j-posy)+(terkep[j][i]-_m)*(terkep[j][i]-_m)<25*25)
                {
                    Vizmagassag v0(i,j,terkep[j][i]);
                    fenyek.insert(v0);
                }
            }
        }

        for (Vizmagassag v: fenyek)
        {
           double hossz = sqrt((v.x-kiindulas.x)*(v.x-kiindulas.x)+(v.y-kiindulas.y)*(v.y-kiindulas.y)+(v.m-kiindulas.m)*(v.m-kiindulas.m));
           if(test(kiindulas,v,terkep))
            gout<<move_to(v.x,v.y)<<color(255,200,3*hossz)<<dot<<refresh; ///refreshelek, mer bár így lassabb a kirajzolás, a grafikuslib képes "lépést tartani" és nem fagy ki.
        }
    }

    void bumm(int posx, int posy, int _m)
    {
        if(terkep[posy][posx]>=_m)
        {
            cout << "Ne akarj a bombaval robbanni! Csak dobd le.\n";
            return;
        }

        int anyag = 0;
        for(int j=posy-25; j<posy+25; j++)
        {
            for(int i=posx-25; i<posx+25; i++)
            {
                if(j<YY && i<XX && i>0 && j>0 && (i-posx)*(i-posx)+(j-posy)*(j-posy)+(terkep[j][i]-_m)*(terkep[j][i]-_m) < 25*25)
                {
                    int a = sqrt((25 * 25) - ((i-posx)*(i-posx)+(j-posy)*(j-posy))) - _m + terkep[j][i];
                    anyag += a; /// mennyi anyag robbant ki
                    terkep[j][i] -= a; /// mennyi anyag maradt az adott részen, a robbanás után
                }
            }
        }

        set<Dist> result;

        for(int j=posy-50; j<posy+50; j++)
        {
            for(int i=posx-50; i<posx+50; i++)
            {
                if(j<YY && i<XX && i>0 && j>0)
                {
                    int a = (i-posx)*(i-posx)+(j-posy)*(j-posy);
                    if (a > 25*25 && a < 50*50) {
                        Dist d0(i,j,a);
                        result.insert(d0); /// hová szórjam el a az anyagot ami kirobbant
                    }
                }
            }
        }

        double unit = (anyag / result.size()) / 2;
        double first = unit * 3;
        double second = unit * 2;
        double third = unit;
        int steps = result.size() / 3;
        int counter = 0;
        for (Dist d: result) /// először megváltoztatom a térkép elemeit,
        {
            if (counter / steps < 1)
                terkep[d.y][d.x] += first;
            else if (counter / steps < 2)
                terkep[d.y][d.x] += second;
            else if (counter / steps < 3)
                terkep[d.y][d.x] += third;
            counter++;
        }

        for(int j=0; j<YY; j++) /// beadom a vizmagassag vektorba a kirajzoláshoz,
        {
            for(int i=0; i<XX; i++)
            {
                Vizmagassag v(i, j, terkep[j][i]);
                vizmagassag.push_back(v);
            }
        }

        kirajzol();

        int counter2 = 0;
        for (Dist d: result) /// majd kirajzolom a helyeket ahová a robbanás elszórta az anyagot.
        {
            if (counter2 / steps < 1)
                gout << move_to(d.x, d.y) << color(255,0,255) << dot;
            else if (counter2 / steps < 2)
                gout << move_to(d.x, d.y) << color(255,125,255) << dot;
            else if (counter2 / steps < 3)
                gout << move_to(d.x, d.y) << color(255,255,255) << dot;
            counter2++;
        }
    }

    bool keres(Vizmagassag k1, Vizmagassag k2) /// irányvektoros felírással menne végig a fényforrásig vezető úton, ha meg lenne hívva. kifagyott tőle a térkép, de az algoritmus lefutott.
    {
        Vizmagassag vvect(k1.x-k2.y, k1.y-k2.y, k1.m-k2.m);
        double hossz = sqrt((k1.x-k2.x)*(k1.x-k2.x)+(k1.y-k2.y)*(k1.y-k2.y)+(k1.m-k2.m)*(k1.m-k2.m));
        Vizmagassag v(vvect.x/hossz,vvect.y/hossz,vvect.m/hossz);
        int i=1;

        while(!(k1.x == k2.x+i*v.x && k1.y == k2.y+i*v.y && k1.m == k2.m+i*v.m) && i<25)
        {
            double z = ((k1.x-k2.x+i*v.x)/v.x)*v.m+k1.m;
            i++;
            if(terkep[k2.y+i*v.y][k2.x+i*v.x]>z)
                return false;
        }
        return true;
    }
};



int main()
{
    terkep_generalas();
    TerkepKezelo terkep;
    gout << refresh;
    int elso_x = 0;
    int elso_y = 0;
    bool space = true;
    bool enter = false;
    bool backspace = false;
    bool tab = true;
    int magassag = 15;
    int feny_x = 0;
    int feny_y = 0;
    cout << "Nyomj entert, hogy hidakat kereshess\nNyomj deletet a terkep eredeti allapotba visszaallitasahoz\nNyomj backspace-t, hogy fenyforrast helyezz el\n";
    cout << "Nyomj TAB-ot hogy robbanthass!\n\n";
    cout << "Ut kereses: elso klikk jobb, masodik klikk bal\n Viz ontes bal klikk\nJobb klikk: indits tengeralattjarol dront\n";

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
                if(tab)
                    terkep.tengeralattjaro(ev.pos_x, ev.pos_y);
                else
                    terkep.bumm(ev.pos_x,ev.pos_y,magassag);
                gout << refresh;
            }

            if(ev.button == btn_left)
            {
                if(space) {
                terkep.tengerszem_keres(ev.pos_x, ev.pos_y);
                terkep.kirajzol();
                bool rajzol = terkep.ut_keres(elso_x, ev.pos_x, elso_y, ev.pos_y);
                if(rajzol)
                    terkep.ut_rajzol(elso_x, ev.pos_x, elso_y, ev.pos_y);
                }
                if(enter){
                    bool hid = terkep.ut_keres(elso_x, ev.pos_x, elso_y, ev.pos_y);
                    terkep.kirajzol();
                    if(!hid)
                        terkep.sziget_szele_keres(elso_x, ev.pos_x, elso_y, ev.pos_y);
                }
                if(backspace) {
                    terkep.fenyforras(ev.pos_x,ev.pos_y,magassag);
                    feny_x = ev.pos_x;
                    feny_y = ev.pos_y;
                }
                gout << refresh;
            }
            if(ev.button == btn_wheelup) /// ha felfelé görgetek
            {
               if(magassag <= 10) {
                } else {
                magassag -=5;
                cout << "A mutato magassaga: "<< magassag << endl;
                }
            terkep.kirajzol();
            terkep.fenyforras(feny_x,feny_y,magassag);
            }

            if(ev.button == btn_wheeldown) /// ha lefelé görgetek
            {
                if(magassag >= 50) {
                } else {
                magassag +=5;
                cout << "A mutato magassaga: "<< magassag << endl;
                }
            terkep.kirajzol();
            terkep.fenyforras(feny_x,feny_y,magassag);
            }
        }

        if(ev.keycode == key_tab) {
            backspace = false;
            space = true;
            enter = false;
            tab = !tab;
            if(tab)
                cout << "Onts vizet a terkepre\n";
            else
                cout << "Mar robbanthatsz!\n";
        }

        if(ev.keycode == key_space) {
            space = true;
            enter = false;
            backspace = false;
            cout << "Kereshetsz utakat\n";
            gout << refresh;
        }
        if(ev.keycode == key_enter) {
            terkep.kirajzol();
            enter = true;
            cout << "Hidakat epithetsz\n";
            space = false;
            backspace = false;
            gout << refresh;
        }
        if(ev.keycode == key_delete){
            terkep.uj_beolvasas();
            terkep.kirajzol();
            gout << refresh;
        }
        if(ev.keycode == key_backspace) {
            enter = false;
            space = false;
            backspace = true;
            cout << "Helyezz el egy fenyforrast az egereddel! A magassagat elozetesen egergorgovel allithatod.\n";
        }
    }

    return 0;
}
