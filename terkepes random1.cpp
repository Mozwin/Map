#include "graphics.hpp"
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>
using namespace genv;
using namespace std;

void beolvas(vector<vector<int> > &v)
{
    ifstream fin("terkep.txt");
    unsigned XX, YY;
    fin >> XX >> YY;
    v.resize(XX);
    for(size_t i = 0; i < XX; ++i)
    {
        v[i].resize(YY);
        for(size_t j = 0; j < YY; ++j)
        {
            fin >> v[i][j];
        }
    }
}

void kirajzol(const vector<vector<int> > &v, canvas& c)
{
    for(size_t i = 0; i < v.size(); ++i)
    {
        for(size_t j = 0; j < v[i].size(); ++j)
        {
            int g,b;
            int norm_h = 127+v[i][j];
            g = norm_h;
            if(v[i][j] < 0) b = 255;
            else b = 0;
            c << move_to(i,j) << color(0,g,b) << dot;
        }
    }
    c << refresh;
}

void tr(canvas &c, const unsigned &W, const unsigned &H,const unsigned &r = 0,const unsigned &g = 0,const unsigned &b = 0){
    c << move_to(0,0) << color(r,g,b) << box(W,H);
}

void tr(canvas& out, canvas& background){
    out << stamp(background,0,0) << refresh;
}

void showmarker(canvas &c, int* marker){
    c << move_to(marker[0]>0?marker[0]-1:marker[0],marker[1]>0?marker[1]-1:marker[1]);
    c << color(255,0,0) << box(marker[0]>0?3:2,marker[1]>0?3:2);
    c << refresh;
}

bool vaneviz(canvas &c, vector<vector<int>> &t, int *from, int* to){
    float r = pow((to[0]-from[0])*(to[0]-from[0])+(to[1]-from[1])*(to[1]-from[1]),0.5);
    float vx = (to[0]-from[0])/r, vy = (to[1]-from[1])/r;
    float x=from[0],y = from[1];
    while((x-to[0])*(x-to[0])+(y-to[1])*(y-to[1])>=1){
        if(t[x][y] <= 0){
            return true;
        }
        c << move_to(x,y) << color(255,0,0)<<dot;
        x+=vx;
        y+=vy;
        gout << refresh;
    }
    return false;
}


int main()
{
    vector<vector<int> > terkep;
    beolvas(terkep);
    unsigned XX = terkep.size();
    unsigned YY = terkep[0].size();
    gout.open(XX,YY);
    canvas hatter(XX,YY);
    kirajzol(terkep,hatter);
    tr(gout, hatter);
    int koord_from[2], koord_to[2];
    koord_from[0] = koord_from[1] = koord_to[0] = koord_to[1] = 0;
    event ev;
    while(gin >> ev && ev.keycode!=key_escape)
    {
        if(ev.type==ev_mouse){
            if(ev.button == btn_left){
                koord_from[0] = ev.pos_x;
                koord_from[1] = ev.pos_y;
                tr(gout,hatter);
                showmarker(gout, koord_from);
                showmarker(gout, koord_to);
            }else if(ev.button == btn_right){
                koord_to[0] = ev.pos_x;
                koord_to[1] = ev.pos_y;
                tr(gout,hatter);
                showmarker(gout, koord_to);
                showmarker(gout, koord_from);
            }
        }
        if(ev.type== ev_key && ev.keycode == key_enter){
            cout << vaneviz(gout,terkep,koord_from,koord_to);

        }
    }
    return 0;
}





