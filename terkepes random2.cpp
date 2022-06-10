#include "graphics.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
using namespace genv;
using namespace std;


//szokasos canvas torlo fuggveny megadhato hatterszinnel.
void tr(canvas &c, const unsigned &W, const unsigned &H,const unsigned &r = 0,const unsigned &g = 0,const unsigned &b = 0){
    c << move_to(0,0) << color(r,g,b) << box(W,H);
}
//torles canvas segitsegevel megadott hatterrel. Feltesszuk, hogy a 2 canvas emrete ugyanakkora.
void tr(canvas& out, canvas& background){
    out << stamp(background,0,0) << refresh;
}

void beolvas(vector<vector<int> > &t){
    ifstream fin("terkep.txt");
    if(!fin.good()){
        cerr << "Nincs meg a bemeneti fajl!\n";
        exit(1);

    }
    int X , Y;
    fin >> X >> Y;
    t.resize(X);
    for(size_t i = 0;i<t.size(); ++i){
        t[i].resize(Y);
        for(size_t j = 0; j < t[i].size(); ++j){
            fin >> t[i][j];
        }
    }
}
void kirajzol(canvas & c, vector<vector <int> > &t){
    for(size_t i = 0; i < t.size(); ++i){
        for(size_t j = 0; j < t[i].size(); ++j){
            int r=0;
            int g,b;
            int col = t[i][j]+127;
            if(t[i][j]<=0){
                g = col/2;
                b = 255;
            } else {
                g = col;
                b = col/2;
            }
            c << move_to(i,j) << color(r,g,b) << dot;
        }
    }
    c << refresh;
}

bool folyo(size_t curr_x, size_t curr_y, vector<vector<int>>&terkep){
    int next_x = -1, next_y = -1;
    int min_h = terkep[curr_x][curr_y];

    for(size_t i = curr_x - (curr_x>0?1:0); i < curr_x+(curr_x<terkep.size()-1?1:0);++i){
        for(size_t j = curr_y- (curr_y>0?1:0); j < curr_y+(curr_y<terkep[i].size()-1?1:0);++j)
            if((i != curr_x || j != curr_y) && terkep[i][j] <= min_h){
                next_x = i;
                next_y = j;
                min_h = terkep[i][j];
            }
    }

    if(next_x != -1){
        if(terkep[next_x][next_y] <= 0){
            return true;
        } else {
            gout << move_to(curr_x,curr_y) << color(0,0,255) << dot;
            return folyo(next_x, next_y, terkep);
        }
    } else return false;
}
bool folyo_it(size_t curr_x, size_t curr_y, vector<vector<int>>&terkep){
    while(curr_x != -1 && terkep[curr_x][curr_y] <= 0){
         int next_x = -1, next_y = -1;
         int min_h = terkep[curr_x][curr_y];
         int mini;
         if(curr_x>0) mini = 1;
         else mini = 0;
         for(size_t i = curr_x - mini; i < curr_x+(curr_x<terkep.size()-1?1:0);++i){
            for(size_t j = curr_y- (curr_y>0?1:0); j < curr_y+(curr_y<terkep[i].size()-1?1:0);++j)
            if((i != curr_x || j != curr_y) && terkep[i][j] <= min_h){
                next_x = i;
                next_y = j;
                min_h = terkep[i][j];
            }
        }
        curr_x = next_x;
        curr_y = next_y;

    }
    if(curr_x != -1){
            return true;
    } else {
        return false;
    }

}


void hegycsucs(size_t curr_x, size_t curr_y,int min_h, vector<vector<int>>&terkep, vector<vector<bool>>&checked){
    if(curr_x < 0 || curr_x > terkep.size() || curr_y < 0 ||
        curr_y > terkep[curr_x].size() || checked[curr_x][curr_y]) return;
    if( terkep[curr_x][curr_y] < min_h){
            gout << move_to(curr_x,curr_y) << color(255,0,0) << dot << refresh;
            return;
    }
    checked[curr_x][curr_y] = true;
    gout << move_to(curr_x,curr_y) << color(255,255,255) << dot << refresh;
    hegycsucs(curr_x-1,curr_y,min_h, terkep, checked);
    hegycsucs(curr_x+1,curr_y,min_h, terkep, checked);
    hegycsucs(curr_x,curr_y-1,min_h, terkep, checked);
    hegycsucs(curr_x,curr_y+1,min_h, terkep, checked);

}

struct koord{
    int x, y;
};

// igazat ad vissza, ha k benne van v-ben, hamisat, ha nem
bool benne_van(koord k, vector<koord> &v) {
    // végigmegyünk az elemeken
    for (int i = 0; i<v.size(); i++) {
        // ha az aktuális elem megegyezik k-val...
        if (v[i].x == k.x && v[i].y == k.y) {
            // akkor megtaláltuk k-t, igazat adunk vissza
            return true;
        }
    }
    // ha végigértünk a tömbön abélkül, hogy megtaláltuk volna k-t, akkor k
    // k nincs benne v-ben, hamisat adunk vissza
    return false;
}

// fehérre színezi a kattintás helye alatti szigetnek a kattintásnál magasabb pontjait
void szinez(vector<vector<int> > &tk, int start_x, int start_y) {
    // a rajzszínt fehérre állítjuk
    gout << color(255, 255, 255);

    // ezeket a pontokat akarjuk még meglátogatni
    vector<koord> sor;
    // ezeket már meglátogattuk
    vector<koord> voltmar;

    // a startpontot belerakjuk a sorba
    koord start = {start_x, start_y};
    sor.push_back(start);

    // addig fusson a ciklus, amíg van még meglátogatandó pont
    while (!sor.empty()) {
        // kivesszük a sor elsõ pontját
        koord p = sor[0];
        sor.erase(sor.begin());

        // megnézzük, hogy p a térképen van-e
        bool terkepen_van = p.x >= 0 && p.x < tk.size() && p.y >= 0 && p.y < tk[0].size();
        // ha nem, akkor kihagyjuk
        if (!terkepen_van) {
            continue;
        }
        // ha igen, akkor megnézzük, hogy elég magas-e
        bool magasfold = tk[p.x][p.y] >= tk[start_x][start_y];
        // és hogy láttuk-e már
        bool meg_nem_volt = !benne_van(p, voltmar);

        // ha ez a két feltétel teljesül
        if (terkepen_van && magasfold && meg_nem_volt) {
            // akkor meglátogatjuk (pirosra színezzük)
            gout << move_to(p.x, p.y)
                 << dot;
            // megjegyezzük, hogy itt már jártunk
            voltmar.push_back(p);
            // és a szomszédait belerakjuk a sorba
            koord bal = {p.x-1, p.y};
            koord jobb = {p.x+1, p.y};
            koord fenti = {p.x, p.y-1};
            koord lenti = {p.x, p.y+1};
            sor.push_back(bal);
            sor.push_back(jobb);
            sor.push_back(fenti);
            sor.push_back(lenti);
        }
    }

    // ha elfogyott a sor, akkor frissítjük a képernyõt és kész vagyunk
    gout << refresh;
}


void kiir(vector<vector<int> > &t){

    ofstream fout("terkep_out.txt");

    int X =t.size(), Y = t[0].size();
    fout << X << " " << Y << endl;
    for(size_t i = 0;i<t.size(); ++i){
        for(size_t j = 0; j < t[i].size(); ++j){
            if(j != 0){
                fout << " ";
            }
            fout << t[i][j];

        }
        fout << endl;
    }

}

int main()
{
    vector<vector<int> > terkep;
    beolvas(terkep);
    unsigned X,Y;
    X = terkep.size();
    Y = terkep[0].size();
    gout.open(X,Y);
    kirajzol(gout,terkep);
    event ev;
    while(gin >> ev && ev.keycode!=key_escape)
    {
        if(ev.type == ev_mouse && ev.button == btn_left){
            cout << folyo(ev.pos_x,ev.pos_y,terkep);
            gout << refresh;
        }
        if(ev.type == ev_mouse && ev.button == btn_right){
            szinez(terkep,ev.pos_x,ev.pos_y);
            gout << refresh;
        }
        if(ev.type == ev_mouse && ev.button == btn_wheelup){
            size_t mini = (ev.pos_x>50?ev.pos_x -50:0), maxi = ((ev.pos_x<terkep.size()-50)?ev.pos_x+50:terkep.size());
            for(size_t i = mini; i < maxi;++i){
                size_t minj = (ev.pos_y>50?ev.pos_y - 50:0), maxj = (ev.pos_y<terkep[i].size()-50?ev.pos_y+50:terkep[i].size());
                for(size_t j = minj; j < maxj ;++j)
                    if(terkep[i][j] < 128){
                            terkep[i][j]++;
                    }
            }
            kirajzol(gout,terkep);
        }

        if(ev.type == ev_mouse && ev.button == btn_wheeldown){
            size_t mini = (ev.pos_x>50?ev.pos_x -50:0), maxi = ((ev.pos_x<terkep.size()-50)?ev.pos_x+50:terkep.size());
            for(size_t i = mini; i < maxi;++i){
                size_t minj = (ev.pos_y>50?ev.pos_y - 50:0), maxj = (ev.pos_y<terkep[i].size()-50?ev.pos_y+50:terkep[i].size());
                for(size_t j = minj; j < maxj ;++j)
                    if(terkep[i][j] > -128){
                            terkep[i][j]--;
                    }
            }
            kirajzol(gout,terkep);
        }
        if(ev.type == ev_key && ev.keycode == key_enter){
            kiir(terkep);
        }

    }
    return 0;
}
