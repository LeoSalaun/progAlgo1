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
}

struct point {
    int x;
    int y;

    void init(int x,int y) {
        this->x = x;
        this->y = y;
    }
}

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
}

void changeDirection(ball, side) {
    
}