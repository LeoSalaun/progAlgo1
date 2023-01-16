#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"

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

void draw(SDL_Renderer* renderer, ellipse ell)
{
    /* Remplissez cette fonction pour faire l'affichage du jeu */
    
    
    filledEllipseRGBA(renderer,ell.coX,ell.coY,ell.radius,ell.radius,ell.red,ell.blue,ell.green,255); 

   
}

bool inEllipse(ellipse ell, SDL_Event e) {
    return sqrt(float( (e.button.x-ell.coX)*(e.button.x-ell.coX) + (e.button.y-ell.coY)*(e.button.y-ell.coY))) <= RADIUS;
}

void ajoutListe(liste* l, ellipse ell) {
    liste * aux = l;
    if (l == nullptr) {
        l = new liste;
        l->init(ell);
    }
    else {
        while (aux->suivant != nullptr) {
            aux = aux->suivant;
        }
        aux->suivant = new liste;
        aux->suivant->init(ell);
    }
}

void handleMouseEvent(liste * l, SDL_Event e) {
    if (e.button.button == SDL_BUTTON_LEFT) {
        if (l != nullptr && inEllipse(l->ell,e)) {
            delete l;
            l = nullptr;
        }
        else if (l != nullptr) {
            liste* auxFollow = l;
            liste* auxRemove = l->suivant;
            bool trouve = false;
            while (auxRemove != nullptr && !(trouve)) {
                trouve = inEllipse(auxRemove->ell,e);
                if (!(trouve)) {
                    auxFollow = auxRemove;
                    auxRemove = auxRemove->suivant;
                }
            }
            if (trouve) {
                auxFollow->suivant = auxRemove->suivant;
                delete auxRemove;
            }
            else {
                ellipse ell;
                ell.init(e.button.x,e.button.y,rand()%10,rand()%10,RADIUS,rand()%255,rand()%255,rand()%255);
                ajoutListe(l,ell);
            }
        }
        else {
            ellipse ell;
            ell.init(e.button.x,e.button.y,rand()%10,rand()%10,RADIUS,rand()%255,rand()%255,rand()%255);
            ajoutListe(l,ell);
        }
    }
}

bool handleEvent(liste * l)
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

void moveEllipse(ellipse *ell) {
    ell->coX += ell->speedX;
    ell->coY += ell->speedY;
}

void handleEventsEllipse(ellipse *ell) {
    if (ell->coX + ell->radius >= SCREEN_WIDTH || ell->coX <= ell->radius) {
        ell->speedX = -ell->speedX;
    }
    if (ell->coY + ell->radius >= SCREEN_HEIGHT || ell->coY <= ell->radius) {
        ell->speedY = -ell->speedY;
    }
}

void initListe(liste* l, int argc, char** argv) {
    ellipse ell;

    if (argc == 1) {
        
        for (int i=0 ; i<9 ; i++) {
            ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%RADIUS+1,rand()%RADIUS+1,RADIUS,rand()%255,rand()%255,rand()%255);

            if (ell.coX-RADIUS < 0) {
                ell.coX = RADIUS;
            }
            else if (ell.coX+RADIUS > SCREEN_WIDTH) {
                ell.coX = SCREEN_WIDTH - RADIUS;
            }

            if (ell.coY-RADIUS < 0) {
                ell.coY = RADIUS;
            }
            else if (ell.coY+RADIUS > SCREEN_HEIGHT) {
                ell.coY = SCREEN_HEIGHT - RADIUS;
            }

            ajoutListe(l,ell);
        }
    }
    else {
        for (int i=1 ; i<argc ; i++) {
            // à compléter ...
        }
    }
    
}

void drawListe(SDL_Renderer * renderer,liste* l) {
    liste * aux = l;
    while (aux != nullptr) {
        draw(renderer,aux->ell);
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

void handleEventsList(liste* l) {
    liste * aux = l;
    while (aux != nullptr) {
        handleEventsEllipse(&(aux->ell));
        aux = aux->suivant;
    }
}