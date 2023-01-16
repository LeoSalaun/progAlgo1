#include "application_ui.h"
#include "SDL2_gfxPrimitives.h"
#include "fonctions.h"

#include <iostream>
using namespace std;

const int RADIUS = 10;

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

    /*ellipse base;
    base.init(0,0,0,0,0,0,0,0);

    listeEllipse->init(base);*/
    liste *listeEllipse = new liste;
    initListe(listeEllipse, argc, argv);

    /*ellipse ell1, ell2, ell3;

    ell1.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%10+1,rand()%10+1,10,rand()%255,rand()%255,rand()%255);
    ell2.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%10+1,rand()%10+1,10,rand()%255,rand()%255,rand()%255);
    ell3.init(rand()%SCREEN_WIDTH,rand()%SCREEN_HEIGHT,rand()%10+1,rand()%10+1,10,rand()%255,rand()%255,rand()%255);
    
    listeEllipse->init(ell1);
    ajoutListe(listeEllipse,ell2);
    ajoutListe(listeEllipse,ell3);*/
    
    /*listeEllipse->ell = ell1;
    listeEllipse->suivant->ell = ell2;
    listeEllipse->suivant->suivant->ell = ell3;*/

    /*  GAME LOOP  */
    while(true)
    {
        // INPUTS
        //SDL_Event e;

        is_running = handleEvent();
        if (!is_running)
            break;
        
        /*switch (e.type)
        {
        case SDL_MouseButtonEvent:
            // ______________
            break;
        
        default:
            break;
        }*/

        // GESTION ACTEURS

        /*moveEllipse(&ell1);
        moveEllipse(&ell2);
        moveEllipse(&ell3);*/

        moveList(listeEllipse);

        /*handleEventsEllipse(&ell1);
        handleEventsEllipse(&ell2);
        handleEventsEllipse(&ell3);*/
        
        handleEventsList(listeEllipse);
        
        // EFFACAGE FRAME
        SDL_SetRenderDrawColor(renderer, 0, 0, 100, 255);
        SDL_RenderClear(renderer);
        
        // DESSIN

        drawListe(renderer,listeEllipse);

        /*draw(renderer,ell1);
        draw(renderer,ell2);
        draw(renderer,ell3);*/

        // VALIDATION FRAME
        SDL_RenderPresent(renderer);

        // PAUSE en ms
        SDL_Delay(1000/30); 
    }

    //Free resources and close SDL
    close(gWindow, renderer);

    return 0;
}
