#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include "fonctions.h"

#include <iostream>
using namespace std;

const int RADIUS = 20;

int main(int argc, char** argv) {
    SDL_Window* gWindow;
    SDL_Renderer* renderer;
    bool is_running = true; 

    // Creation de la fenetre
    gWindow = init("Awesome Game");

    if (!gWindow)
    {
        SDL_Log("Failed to initialize!\n");
        exit(1);
    }

    renderer = SDL_CreateRenderer(gWindow, -1, 0); // SDL_RENDERER_PRESENTVSYNC


    // CREATION DES ACTEURS

    level lvl;

    if (argc > 1 && strcmp(argv[1],"level") == 0) {
        if (argc > 2 && argv[2][1] != 'x' && strcmp(argv[2],"level") != 0) {
            if (atoi(argv[2]) <= 0) {
                lvl.init(0,0,0,0,11,15);
            }
            else {
                switch (atoi(argv[2])) {
                    case 1 : lvl.init(1,0,0,100,1,4);
                    break;
                    case 2 : lvl.init(2,100,0,100,5,7);
                    break;
                    default : lvl.init(3,100,0,0,8,10);
                    break;
                }
            }
        }
        else {
            lvl.init(1,0,0,100,1,4);
        }
    }
    else {
        lvl.init(1,0,0,100,1,4);
    }

    int colorGrow;
    if (lvl.num == 0) {
        colorGrow = 15;
    }
    else {
        colorGrow = 0;
    }

    wall murs[4];

    if (lvl.num != 0) {
        murs[0].init(50,100,275,125);
        murs[1].init(600,75,635,260);
        murs[2].init(400,360,650,400);
        murs[3].init(155,220,165,410);
    }

    liste * listeEllipse = nullptr;

    initListe(&listeEllipse, murs, argc, argv, lvl);

    /*  GAME LOOP  */
    while(true)
    {
        // INPUTS
        
        is_running = handleEvent(&listeEllipse,lvl);
        if (!is_running)
            break;

        // GESTION ACTEURS

        moveList(listeEllipse,lvl);
        
        handleEventsList(listeEllipse,murs,lvl);

        
        colorGrow++;

        if (lvl.num == 0) {
            if ((colorGrow >= 40 && colorGrow <= 42) || (colorGrow >= 30 && colorGrow <= 32)) {
                lvl.r+=2;
                lvl.g+=2;
                lvl.b+=2;
            }
            else if ((colorGrow >= 43 && colorGrow <= 44) || (colorGrow >= 33 && colorGrow <= 34)) {
                lvl.r+=10;
                lvl.g+=10;
                lvl.b+=10;
            }
            else if ((colorGrow >= 45 && colorGrow <= 46) || (colorGrow >= 35 && colorGrow <= 36)) {
                lvl.r-=10;
                lvl.g-=10;
                lvl.b-=10;
            }
            else if ((colorGrow >= 47 && colorGrow <= 49) || (colorGrow >= 37 && colorGrow <= 39)) {
                lvl.r-=2;
                lvl.g-=2;
                lvl.b-=2;
            }

            if (colorGrow == 50) {
                colorGrow = 0;
                lvl.r = 0;
                lvl.g = 0;
                lvl.b = 0;
            }
        }
        else {
            if (colorGrow >= 0 && colorGrow <= 24) {
                lvl.r++;
                lvl.g++;
                lvl.b++;
            }
            else if (colorGrow >= 25 && colorGrow <= 49) {
                lvl.r--;
                lvl.g--;
                lvl.b--;
            }
            if (colorGrow == 50) {
                colorGrow = 0;
            }
        }
        
        // EFFACAGE FRAME
        SDL_SetRenderDrawColor(renderer, lvl.r, lvl.g, lvl.b, 255);
        SDL_RenderClear(renderer);
        
        // DESSIN

        drawListe(renderer,listeEllipse);
        
        if (lvl.num != 0) {
            for (int i=0 ; i<4 ; i++) {
                drawWall(renderer,murs[i]);
            }
        }

        // VALIDATION FRAME
        SDL_RenderPresent(renderer);

        // PAUSE en ms
        SDL_Delay(1000/30); 
    }

    //Free resources and close SDL
    close(gWindow, renderer);

    return 0;
}
