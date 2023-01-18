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

    wall murs[4];
    murs[0].init(50,100,275,125);
    murs[1].init(600,75,635,260);
    murs[2].init(400,360,650,400);
    murs[3].init(155,220,165,410);

    liste * listeEllipse = nullptr;

    /*
    liste * listeEllipse = new liste;
    ellipse ell;
    ell.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%10+1,rand()%10+1,RADIUS,rand()%155+100,rand()%155+100,rand()%155+100);
    correctCoordinates(&ell,murs);
    listeEllipse->init(ell);*/

    initListe(&listeEllipse, murs, argc, argv);

    /*ellipse ell1, ell2, ell3;

    ell1.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%10+1,rand()%10+1,10,rand()%255,rand()%255,rand()%255);
    ell2.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%10+1,rand()%10+1,10,rand()%255,rand()%255,rand()%255);
    ell3.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%10+1,rand()%10+1,10,rand()%255,rand()%255,rand()%255);
    
    listeEllipse->init(ell1);
    ajoutListe(listeEllipse,ell2);
    ajoutListe(listeEllipse,ell3);*/

    /*  GAME LOOP  */
    while(true)
    {
        // INPUTS
        
        is_running = handleEvent(&listeEllipse);
        if (!is_running)
            break;

        // GESTION ACTEURS

        moveList(listeEllipse);
        
        handleEventsList(listeEllipse,murs);
        
        // EFFACAGE FRAME
        SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
        SDL_RenderClear(renderer);
        
        // DESSIN

        drawListe(renderer,listeEllipse);
        
        for (int i=0 ; i<4 ; i++) {
            drawWall(renderer,murs[i]);
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
