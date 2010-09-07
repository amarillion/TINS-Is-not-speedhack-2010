#include "colours.hpp"
#include <allegro.h>

int Colour::black;
int Colour::white;
int Colour::red;
int Colour::green;
int Colour::blue;
int Colour::yellow;
int Colour::purple;
int Colour::turquoise;
int Colour::blood_red1;
int Colour::blood_red2;
int Colour::blood_red3;
int Colour::gui_gold;
int Colour::carbon;
int Colour::magic_pink;


void Colour::Init()
{
    black = makecol(0,0,0);
    white = makecol(255,255,255);
    red = makecol(255,0,0);
    green = makecol(0,255,0);
    blue = makecol(0,0,255);
    yellow = makecol(255,255,0);
    purple = makecol(128,0,128);
    carbon = makecol(20,20,20);
    turquoise = makecol(0,255,239);

    blood_red1 = makecol(100,0,0);
    blood_red2 = makecol(120,0,0);
    blood_red3 = makecol(140,0,0);

    gui_gold = makecol(150,113,23);

    magic_pink = makecol(255,0,255);
}
