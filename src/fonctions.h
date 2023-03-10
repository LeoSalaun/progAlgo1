#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include <typeinfo>
#include <string.h>
#include <cstdlib>

#include <iostream>
using namespace std;

struct ellipse {
    int coX;
    int coY;
    int speedX;
    int speedY;
    int radius;
    unsigned int red;
    unsigned int green;
    unsigned int blue;

    void init(int coX,int coY,int speedX,int speedY,int radius,unsigned int red,unsigned int green,unsigned int blue) {
        this->coX = coX;
        this->coY = coY;
        this->speedX = speedX;
        this->speedY = speedY;
        this->radius = radius;
        this->red = red;
        this->green = green;
        this->blue = blue;
    }
};

struct point {
    int x;
    int y;

    void init(int x,int y) {
        this->x = x;
        this->y = y;
    }
};

struct wall {
    point point1;
    point point2;

    void init(int x1,int y1,int x2,int y2) {
        point1.init(x1,y1);
        point2.init(x2,y2);
    }

    void init(point point1, point point2) {
        this->point1 = point1;
        this->point2 = point2;
    }
};

struct liste {
    ellipse ell;
    liste * suivant;

    void init(ellipse first) {
        this->ell = first;
        this->suivant = nullptr;
    }
};

struct level {
    int num;
    int r;
    int g;
    int b;
    int minSpeed;
    int maxSpeed;

    void init(int num, int r, int g, int b, int minSpeed, int maxSpeed) {
        this->num = num;
        this->r = r;
        this->g = g;
        this->b = b;
        this->minSpeed = minSpeed;
        this->maxSpeed = maxSpeed;
    }
};

void drawEllipse(SDL_Renderer* renderer, ellipse ell);

void drawWall(SDL_Renderer* renderer, wall mur);

double distance(int x1, int y1, int x2, int y2);

bool inEllipse(ellipse ell, SDL_Event e);
bool inListe(liste * l, SDL_Event e);

void ajoutListe(liste ** l, ellipse ell);

void removeEllipse(liste ** l, SDL_Event e);

void handleMouseEvent(liste ** l, SDL_MouseButtonEvent e);

bool handleEvent(liste ** l, level lvl);

void correctCoordinatesWindow(ellipse *ell, wall murs[]);

void correctCoordinates(ellipse *ell, wall murs[]);

void moveEllipse(ellipse *ell, level lvl);

bool contactWallX(ellipse ell, wall murs[], level lvl);

bool contactWallY(ellipse ell, wall murs[], level lvl);

bool contactWallAngle(ellipse ell, wall murs[], level lvl);

void handleEventsEllipse(ellipse *ell, wall murs[], level lvl);

bool isInt(char* array);

void initListe(liste ** l, wall murs[], int argc, char** argv, level lvl);

void drawListe(SDL_Renderer * renderer,liste* l);

void moveList(liste* l, level lvl);

void handleEventsList(liste* l, wall murs[], level lvl);