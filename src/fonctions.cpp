#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include <typeinfo>
#include <string.h>
#include <cstdlib>

#include <iostream>
using namespace std;

const int RADIUS = 20;

const int NBCOLORS = 8;
const char COLORS[NBCOLORS][8] = {"red","green","blue","yellow","magenta","cyan","black","white"};
const int COLORSCODE[NBCOLORS][3] = {{255,100,100},{100,255,100},{100,100,255},{255,255,100},{255,100,255},{100,255,255},{100,100,100},{255,255,255}};

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

void drawEllipse(SDL_Renderer* renderer, ellipse ell)
{
    /* Remplissez cette fonction pour faire l'affichage du jeu */
    
    
    filledEllipseRGBA(renderer,ell.coX,ell.coY,ell.radius,ell.radius,ell.red,ell.green,ell.blue,255); 

    circleRGBA(renderer,ell.coX,ell.coY,ell.radius,0,0,0,255);
}

void drawWall(SDL_Renderer* renderer, wall mur) {
    //rectangleRGBA (renderer, mur.point1.x, mur.point1.y, mur.point2.x, mur.point2.y, 150, 150, 150, 255);
    //filledRectAlpha(renderer, mur.point1.x, mur.point1.y, mur.point2.x, mur.point2.y, 0xAAAAAAFF);

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

void removeEllipse(liste ** l, SDL_Event e) {
    /*if (l->suivant == nullptr && inEllipse(l->ell,e)) {
        delete l;
        l = nullptr;
        exit(0);
    }*/

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
            ell.init(e.button.x,e.button.y,rand()%20-10,rand()%20-10,RADIUS,rand()%155+100,rand()%155+100,rand()%155+100);
            ajoutListe(l,ell);
        }
    }
}

void handleMouseEvent(liste ** l, SDL_Event e) {
    if (e.button.button == SDL_BUTTON_LEFT) {
        /*if (l != nullptr && inEllipse(l->ell,e)) {
            delete l;
            l = nullptr;
        }
        else */if ((*l) != nullptr) {
            removeEllipse(l,e);
        }
        else {
            ellipse ell;
            ell.init(e.button.x,e.button.y,rand()%20-10,rand()%20-10,RADIUS,rand()%155+100,rand()%155+100,rand()%155+100);
            ajoutListe(l,ell);
        }
    }
}

bool handleEvent(liste ** l)
{
    /* Remplissez cette fonction pour gérer les inputs utilisateurs */
    SDL_Event e; 
    while (SDL_PollEvent(&e)){ 
        if (e.type == SDL_QUIT) {
            return false;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            handleMouseEvent(l,e);
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

void moveEllipse(ellipse *ell) {
    ell->coX += ell->speedX;
    ell->coY += ell->speedY;

    correctCoordinatesWindow(ell);
}

bool contactWallX(ellipse ell, wall murs[]) {
    bool res = false;
    int i=0;
    while (i<4 && !(res)) {
        res = (ell.coY <= murs[i].point2.y && ell.coY >= murs[i].point1.y && ((ell.coX+RADIUS >= murs[i].point1.x && ell.coX <= (murs[i].point2.x + murs[i].point1.x)/2) || (ell.coX-RADIUS <= murs[i].point2.x && ell.coX >= (murs[i].point2.x + murs[i].point1.x)/2)));
        i++;
    }
    return res;
}

bool contactWallY(ellipse ell, wall murs[]) {
    bool res = false;
    int i=0;
    while (i<4 && !(res)) {
        res = (ell.coX <= murs[i].point2.x && ell.coX >= murs[i].point1.x && ((ell.coY+RADIUS >= murs[i].point1.y && ell.coY <= (murs[i].point2.y + murs[i].point1.y)/2) || (ell.coY-RADIUS <= murs[i].point2.y && ell.coY >= (murs[i].point2.y + murs[i].point1.y)/2)));
        i++;
    }
    return res;
}

bool contactWallAngle(ellipse ell, wall murs[]) {
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

void handleEventsEllipse(ellipse *ell, wall murs[]) {
    if (ell->coX + ell->radius >= SCREEN_WIDTH || ell->coX <= ell->radius || contactWallX(*ell,murs)) {
        ell->speedX = -ell->speedX;
    }
    else if (ell->coY + ell->radius >= SCREEN_HEIGHT || ell->coY <= ell->radius || contactWallY(*ell,murs)) {
        ell->speedY = -ell->speedY;
    }
    else if (contactWallAngle(*ell,murs)) {
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

int color(char* array) {
    bool res = false;
    int i=0;
    while (!(res) && i<NBCOLORS) {
        res = (strcmp(array,COLORS[i]) == 0);
        if (!(res)) {
            i++;
        }
    }
    if (!(res)) {
        i = -1;
    }
    return i;
}

void initListe(liste** l, wall murs[], int argc, char** argv) {
    ellipse ell;
    int indexColor;

    if (argc == 1) {
        
        for (int i=0 ; i<10 ; i++) {
            ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%20-10,rand()%20-10,RADIUS,rand()%155+100,rand()%155+100,rand()%155+100);

            correctCoordinates(&ell,murs);

            ajoutListe(l,ell);
        }
    }
    else {
        /*for (int i=1 ; i<argc ; i++) {
            if (isInt(argv[i])) {*/
                /*ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%RADIUS+1,rand()%RADIUS+1,RADIUS,255,255,255);
                ajoutListe(l,ell);*/
            if (argc == 2) {
                /*if (atoi(argv[1]) <= 0) {
                    delete (*l);
                    (*l) = nullptr;
                }
                else {*/
                    for (int i=0 ; i<atoi(argv[1]) ; i++) {
                        ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%20-10,rand()%20-10,RADIUS,rand()%155+100,rand()%155+100,rand()%155+100);

                        correctCoordinates(&ell, murs);

                        ajoutListe(l,ell);
                    }
                //}
            }
        /*}
            else {
                indexColor = color(argv[i]);
                if (indexColor != -1) {
                    if (i+1<argc && isInt(argv[i+1])) {
                        for (int j=0 ; j<atoi(argv[i+1]) ; j++) {
                            ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%RADIUS+1,rand()%RADIUS+1,RADIUS,COLORSCODE[indexColor][0],COLORSCODE[indexColor][1],COLORSCODE[indexColor][2]);

                            correctCoordinates(&ell, murs);

                            ajoutListe(l,ell);
                        }
                        i++;
                    }
                    else {
                        ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%RADIUS+1,rand()%RADIUS+1,RADIUS,COLORSCODE[indexColor][0],COLORSCODE[indexColor][1],COLORSCODE[indexColor][2]);

                        correctCoordinates(&ell, murs);

                        ajoutListe(l,ell);
                    }
                }
            }
        }
    }*/
        else {
            /*delete l;
            l = nullptr;*/
            for (int i=1 ; i<argc ; i++) {
                indexColor = color(argv[i]);
                /*int r,g,b;
                r = (strtol(argv[i],NULL,16) & 0xFF) >> 16;
                g = (strtol(argv[i],NULL,16) & 0xFF) >> 8;
                b = (strtol(argv[i],NULL,16) & 0xFF) >> 0;*/
                int intColor = strtol(argv[i],NULL,16);

                int r,g,b;

                /*r[0] = argv[i][2];
                r[1] = argv[i][3];

                g[0] = argv[i][4];
                g[1] = argv[i][5];
                
                b[0] = argv[i][6];
                b[1] = argv[i][7];*/

                r = (intColor & 0xFF0000) >> 16;
                g = (intColor & 0x00FF00) >> 8;
                b = (intColor & 0x0000FF) >> 0;

                //ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%RADIUS+1,rand()%RADIUS+1,RADIUS,COLORSCODE[indexColor][0],COLORSCODE[indexColor][1],COLORSCODE[indexColor][2]);
                ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%20-10,rand()%20-10,RADIUS,r,g,b);
                //ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%20-10,rand()%20-10,RADIUS,strtol(r,NULL,16),strtol(g,NULL,16),strtol(b,NULL,16));

                correctCoordinates(&ell, murs);

                ajoutListe(l,ell);
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

void moveList(liste* l) {
    liste * aux = l;
    while (aux != nullptr) {
        moveEllipse(&(aux->ell));
        aux = aux->suivant;
    }
}

void handleEventsList(liste* l, wall murs[]) {
    liste * aux = l;
    while (aux != nullptr) {
        handleEventsEllipse(&(aux->ell),murs);
        aux = aux->suivant;
    }
}