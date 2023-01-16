#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"

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

void draw(SDL_Renderer* renderer, ellipse ell);

bool inEllipse(ellipse ell, SDL_MouseButtonEvent e);

void ajoutListe(liste* l, ellipse ell);

void handleMouseEvent(liste * l, SDL_MouseButtonEvent e);

bool handleEvent(liste * l);

void moveEllipse(ellipse *ell);

void handleEventsEllipse(ellipse *ell);

void initListe(liste* l, int argc, char** argv);

void drawListe(SDL_Renderer * renderer,liste* l);

void moveList(liste* l);

void handleEventsList(liste* l);