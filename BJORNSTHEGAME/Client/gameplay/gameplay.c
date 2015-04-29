#include "gameplay.h"

void gameplayWindow()
{
    SDL_Window* gameplay;
    SDL_Surface* gameSurface;
    SDL_Surface* gameBackground;

    if(SDL_Init(SDL_INIT_VIDEO)<0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        //Create a window
        gameplay = SDL_CreateWindow("BJORNSGAMEPLAY", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    1920,1080,
                                    SDL_WINDOW_FULLSCREEN);
        if(gameplay == NULL){
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else{
            //Get window surface
            gameSurface = SDL_GetWindowSurface(gameplay);

            gameBackground = IMG_Load("gameplay.png");
            SDL_BlitScaled(gameBackground, NULL, gameSurface, NULL);

            //Update the surface
            SDL_UpdateWindowSurface(gameplay);
        }
    }
    SDL_Delay(2000);
    //Destroy window
    SDL_DestroyWindow(gameplay);
    //Quit SDL subsystems
    SDL_Quit();
}

