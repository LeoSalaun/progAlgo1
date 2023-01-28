#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include <typeinfo>
#include <string.h>
#include <cstdlib>

#include <iostream>
using namespace std;

const int RADIUS = 20;

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

void drawEllipse(SDL_Renderer* renderer, ellipse ell)
{
    /* Remplissez cette fonction pour faire l'affichage du jeu */
    
    
    filledEllipseRGBA(renderer,ell.coX,ell.coY,ell.radius,ell.radius,ell.red,ell.green,ell.blue,255); 

    circleRGBA(renderer,ell.coX,ell.coY,ell.radius,0,0,0,255);
}

void drawWall(SDL_Renderer* renderer, wall mur) {

    SDL_Rect rectangle;
    rectangle.x = mur.point1.x;
    rectangle.y = mur.point1.y;
    rectangle.w = mur.point2.x - mur.point1.x;
    rectangle.h = mur.point2.y - mur.point1.y;
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer,&rectangle);

    lineRGBA(renderer, mur.point1.x, mur.point1.y, mur.point2.x, mur.point1.y, 0, 0, 0, 255);
    lineRGBA(renderer, mur.point1.x, mur.point1.y, mur.point1.x, mur.point2.y, 0, 0, 0, 255);
    lineRGBA(renderer, mur.point1.x, mur.point2.y, mur.point2.x, mur.point2.y, 0, 0, 0, 255);
    lineRGBA(renderer, mur.point2.x, mur.point1.y, mur.point2.x, mur.point2.y, 0, 0, 0, 255);
}

double distance(int x1, int y1, int x2, int y2) {
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

bool inEllipse(ellipse ell, SDL_Event e) {
    return distance(e.button.x,e.button.y,ell.coX,ell.coY) <= RADIUS;
}

bool inListe(liste * l, SDL_Event e) {
    liste * aux = l;
    bool res = inEllipse(aux->ell,e);
    while (aux != nullptr && !(res)) {
        aux = aux->suivant;
        if (aux != nullptr) {
            res = inEllipse(aux->ell,e);
        }
    }
    return res;
}

void ajoutListe(liste** l, ellipse ell) {
    if ((*l) == nullptr) {
        liste * aux = new liste;
        aux->init(ell);
        (*l) = aux;
    }
    else {
        liste * aux = (*l);
        while (aux->suivant != nullptr) {
            aux = aux->suivant;
        }
        aux->suivant = new liste;
        aux->suivant->init(ell);
    }
}

void removeEllipse(liste ** l, SDL_Event e, level lvl) {

    if (inEllipse((*l)->ell,e)) {
        liste * aux = (*l);
        (*l) = (*l)->suivant;
        delete aux;
        aux = nullptr;
    }
    else {
        liste* auxFollow = (*l);
        liste* auxRemove = (*l)->suivant;
        bool trouve = false;
        while (auxRemove != nullptr && !(trouve)) {
            trouve = inEllipse(auxRemove->ell,e);
            if (!(trouve)) {
                auxFollow = auxFollow->suivant;
                auxRemove = auxRemove->suivant;
            }
        }
        if (trouve) {
            auxFollow->suivant = auxRemove->suivant;
            delete auxRemove;
        }
        else {
            ellipse ell;
            int signex = rand()%2, signey = rand()%2;
            if (signex == 0) {
                signex = -1;
            }
            if (signey == 0) {
                signey = -1;
            }
            
            if (lvl.num != 0) {
                ell.init(e.button.x,
                        e.button.y,
                        (rand()%5+1)*signex,
                        (rand()%5+1)*signey,
                        RADIUS,
                        rand()%155+100,
                        rand()%155+100,
                        rand()%155+100);
            }
            else {
                ell.init(e.button.x,
                        e.button.y,
                        (rand()%5+1)*signex,
                        (rand()%5+1)*signey,
                        RADIUS,
                        255,
                        255,
                        255);
            }
        }
    }
}

void handleMouseEvent(liste ** l, SDL_Event e, level lvl) {
    if (e.button.button == SDL_BUTTON_LEFT) {
        if ((*l) != nullptr && inListe(*l,e)) {
            removeEllipse(l,e,lvl);
        }
        else {
            ellipse ell;
            int signex = rand()%2, signey = rand()%2;
            if (signex == 0) {
                signex = -1;
            }
            if (signey == 0) {
                signey = -1;
            }

            if (lvl.num != 0) {
                ell.init(e.button.x,
                        e.button.y,
                        (rand()%5+1)*signex,
                        (rand()%5+1)*signey,
                        RADIUS,
                        rand()%155+100,
                        rand()%155+100,
                        rand()%155+100);
            }
            else {
                ell.init(e.button.x,
                        e.button.y,
                        (rand()%5+1)*signex,
                        (rand()%5+1)*signey,
                        RADIUS,
                        255,
                        255,
                        255);
            }

            ajoutListe(l,ell);
        }
    }
}

bool handleEvent(liste ** l, level lvl)
{
    /* Remplissez cette fonction pour gérer les inputs utilisateurs */
    SDL_Event e; 
    while (SDL_PollEvent(&e)){ 
        if (e.type == SDL_QUIT) {
            return false;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            handleMouseEvent(l,e,lvl);
        }
    }
    return true;
}

void correctCoordinatesWindow(ellipse *ell) {
    if (ell->coX-RADIUS < 0) {
        ell->coX = RADIUS;
    }
    else if (ell->coX+RADIUS > SCREEN_WIDTH) {
        ell->coX = SCREEN_WIDTH - RADIUS;
    }

    if (ell->coY-RADIUS < 0) {
        ell->coY = RADIUS;
    }
    else if (ell->coY+RADIUS > SCREEN_HEIGHT) {
        ell->coY = SCREEN_HEIGHT - RADIUS;
    }
}

void correctCoordinates(ellipse *ell, wall murs[]) {

    correctCoordinatesWindow(ell);

    for (int i=0 ; i<4 ; i++) {
        if (ell->coX-RADIUS <= murs[i].point2.x && ell->coX >= (murs[i].point2.x + murs[i].point1.x)/2) {
            ell->coX = murs[i].point2.x + RADIUS;
        }
        else if (ell->coX+RADIUS >= murs[i].point2.x && ell->coX <= (murs[i].point2.x + murs[i].point1.x)/2) {
            ell->coX = murs[i].point2.x - RADIUS;
        }

        if (ell->coY-RADIUS <= murs[i].point2.y && ell->coY >= (murs[i].point2.y + murs[i].point1.y)/2) {
            ell->coY = murs[i].point2.y + RADIUS;
        }
        else if (ell->coY+RADIUS >= murs[i].point1.y && ell->coY <= (murs[i].point2.y + murs[i].point1.y)/2) {
            ell->coY = murs[i].point1.y - RADIUS;
        }
    }
}

void moveEllipse(ellipse *ell, level lvl) {
    if (lvl.num != 0) {
        ell->coX += ell->speedX*lvl.num;
        ell->coY += ell->speedY*lvl.num;
    }
    else {
        ell->coX += ell->speedX*5;
        ell->coY += ell->speedY*5;
    }

    int changeSpeed = rand()%100;

    if (lvl.num == 0 && changeSpeed<5) {
        int signex = rand()%2, signey = rand()%2;
        if (signex == 0) {
            signex = -1;
        }
        if (signey == 0) {
            signey = -1;
        }
        ell->speedX = (rand()%5+1)*signex;
        ell->speedY = (rand()%5+1)*signey;
    }

    /*
    if (lvl.num != 0) {
        if (ell->speedX < 0) {
            ell->coX += ell->speedX-5*(lvl.num-1);
        }
        else {
            ell->coX += ell->speedX+5*(lvl.num-1);
        }

        if (ell->speedX < 0) {
            ell->coY += ell->speedY-5*(lvl.num-1);
        }
        else {
            ell->coY += ell->speedY+5*(lvl.num-1);
        }
    }
    else {
        if (ell->speedX < 0) {
            ell->coX += ell->speedX-20;
        }
        else {
            ell->coX += ell->speedX+20;
        }

        if (ell->speedX < 0) {
            ell->coY += ell->speedY-20;
        }
        else {
            ell->coY += ell->speedY+20;
        }
    }
    */

    correctCoordinatesWindow(ell);
}

bool contactWallX(ellipse ell, wall murs[], level lvl) {
    if (lvl.num == 0) {
        return false;
    }
    bool res = false;
    int i=0;
    while (i<4 && !(res)) {
        res = (ell.coY <= murs[i].point2.y && ell.coY >= murs[i].point1.y && ((ell.coX+RADIUS >= murs[i].point1.x && ell.coX <= (murs[i].point2.x + murs[i].point1.x)/2) || (ell.coX-RADIUS <= murs[i].point2.x && ell.coX >= (murs[i].point2.x + murs[i].point1.x)/2)));
        i++;
    }
    return res;
}

bool contactWallY(ellipse ell, wall murs[], level lvl) {
    if (lvl.num == 0) {
        return false;
    }
    bool res = false;
    int i=0;
    while (i<4 && !(res)) {
        res = (ell.coX <= murs[i].point2.x && ell.coX >= murs[i].point1.x && ((ell.coY+RADIUS >= murs[i].point1.y && ell.coY <= (murs[i].point2.y + murs[i].point1.y)/2) || (ell.coY-RADIUS <= murs[i].point2.y && ell.coY >= (murs[i].point2.y + murs[i].point1.y)/2)));
        i++;
    }
    return res;
}

bool contactWallAngle(ellipse ell, wall murs[], level lvl) {
    if (lvl.num == 0) {
        return false;
    }
    bool res = false;
    int i=0;
    while (i<4 && !(res)) {
        res = (distance(ell.coX,ell.coY,murs[i].point1.x,murs[i].point1.y) <= RADIUS
                || distance(ell.coX,ell.coY,murs[i].point1.x,murs[i].point2.y) <= RADIUS
                || distance(ell.coX,ell.coY,murs[i].point2.x,murs[i].point1.y) <= RADIUS
                || distance(ell.coX,ell.coY,murs[i].point2.x,murs[i].point2.y) <= RADIUS);
        i++;
    }
    return res;
}

void handleEventsEllipse(ellipse *ell, wall murs[], level lvl) {
    if (ell->coX + ell->radius >= SCREEN_WIDTH || ell->coX <= ell->radius || contactWallX(*ell,murs,lvl)) {
        ell->speedX = -ell->speedX;
    }
    else if (ell->coY + ell->radius >= SCREEN_HEIGHT || ell->coY <= ell->radius || contactWallY(*ell,murs,lvl)) {
        ell->speedY = -ell->speedY;
    }
    else if (contactWallAngle(*ell,murs,lvl)) {
        ell->speedX = -ell->speedX;
        ell->speedY = -ell->speedY;
    }
}

bool isInt(char* array) {
    bool res = true;
    int i=0;
    while ((res) && i<static_cast<int>(strlen(array))) {
        res = (array[i] >= 0 && array[i] <= 9);
    }
    return res;
}

void initListe(liste** l, wall murs[], int argc, char** argv, level lvl) {
    ellipse ell;
    int n=0;
    int signex, signey;

    if (argc == 1 || (argc == 2 && strcmp(argv[1],"level") == 0) || (argc == 3 && strcmp(argv[1],"level") == 0 && strcmp(argv[2],"level") != 0 && argv[2][1] != 'x')) {
        
        for (int i=0 ; i<10 ; i++) {
            signex = rand()%2;
            signey = rand()%2;
            if (signex == 0) {
                signex = -1;
            }
            if (signey == 0) {
                signey = -1;
            }

            if (lvl.num != 0) {
                ell.init(rand()%SCREEN_WIDTH,
                        rand()%SCREEN_HEIGHT,
                        (rand()%5+1)*signex,
                        (rand()%5+1)*signey,
                        RADIUS,
                        rand()%155+100,
                        rand()%155+100,
                        rand()%155+100);
            }
            else {
                ell.init(rand()%SCREEN_WIDTH,
                        rand()%SCREEN_HEIGHT,
                        (rand()%5+1)*signex,
                        (rand()%5+1)*signey,
                        RADIUS,
                        255,
                        255,
                        255);
            }

            correctCoordinates(&ell,murs);

            ajoutListe(l,ell);
        }
    }
    else {
        for (int i=1 ; i<argc ; i++) {
            if (argv[i][1] == 'x') {

                int r,g,b;
                
                if (lvl.num != 0) {
                    int intColor = strtol(argv[i],NULL,16);

                    r = (intColor & 0xFF0000) >> 16;
                    g = (intColor & 0x00FF00) >> 8;
                    b = (intColor & 0x0000FF) >> 0;
                }
                else {
                    r = 255;
                    g = 255;
                    b = 255;
                }

                if (i < argc-1 && argv[i+1][1] != 'x' && strcmp(argv[i],"level") != 0) {
                    n = atoi(argv[i+1]);
                    i++;
                }
                else {
                    n = 1;
                }

                for (int j=0 ; j<n ; j++) {
                    signex = rand()%2;
                    signey = rand()%2;
                    if (signex == 0) {
                        signex = -1;
                    }
                    if (signey == 0) {
                        signey = -1;
                    }

                    ell.init(rand()%SCREEN_WIDTH,
                            rand()%SCREEN_HEIGHT,
                            (rand()%5+1)*signex,
                            (rand()%5+1)*signey,
                            RADIUS,
                            r,
                            g,
                            b);

                    correctCoordinates(&ell, murs);

                    ajoutListe(l,ell);
                }
            }
            else if (strcmp(argv[i],"level") == 0 && i < argc-1 && argv[i+1][1] != 'x' && strcmp(argv[i+1],"level") != 0) {
                i++;
            }
            else {
                for (int j=0 ; j<atoi(argv[i]) ; j++) {
                    signex = rand()%2;
                    signey = rand()%2;
                    if (signex == 0) {
                        signex = -1;
                    }
                    if (signey == 0) {
                        signey = -1;
                    }

                    if (lvl.num != 0) {
                        ell.init(rand()%SCREEN_WIDTH,
                                rand()%SCREEN_HEIGHT,
                                (rand()%5+1)*signex,
                                (rand()%5+1)*signey,
                                RADIUS,
                                rand()%155+100,
                                rand()%155+100,
                                rand()%155+100);
                    }
                    else {
                        ell.init(rand()%SCREEN_WIDTH,
                                rand()%SCREEN_HEIGHT,
                                (rand()%5+1)*signex,
                                (rand()%5+1)*signey,
                                RADIUS,
                                255,
                                255,
                                255);
                    }

                    correctCoordinates(&ell, murs);

                    ajoutListe(l,ell);
                }
            }
        }
    }
    
}

void drawListe(SDL_Renderer * renderer,liste* l) {
    liste * aux = l;
    while (aux != nullptr) {
        drawEllipse(renderer,aux->ell);
        aux = aux->suivant;
    }
}

void moveList(liste* l, level lvl) {
    liste * aux = l;
    while (aux != nullptr) {
        moveEllipse(&(aux->ell),lvl);
        aux = aux->suivant;
    }
}

void handleEventsList(liste* l, wall murs[], level lvl) {
    liste * aux = l;
    while (aux != nullptr) {
        handleEventsEllipse(&(aux->ell),murs,lvl);
        aux = aux->suivant;
    }
}