#include "gameplay.h"

void gameplayWindow()
{
    SDL_Window* gameplay;
    SDL_Surface* gameSurface;
    SDL_Surface* gameBackground = IMG_Load("gameplay.png");;
    SDL_Surface* ground =IMG_Load("ground.png");
    SDL_Surface* platform1 = IMG_Load("platform_hor.png");
    SDL_Surface* platform2 = IMG_Load("platform_ver.png");
    SDL_Surface* band = IMG_Load("band.png");
    SDL_Surface* bjrons = IMG_Load("bjorndrapare.png");





    if(SDL_Init(SDL_INIT_VIDEO)<0){
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        //Create a window
        gameplay = SDL_CreateWindow("BJORNSGAMEPLAY", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                    1920,1080,
                                    SDL_WINDOW_FULLSCREEN_DESKTOP);
        if(gameplay == NULL){
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else{
            //Get window surface
            gameSurface = SDL_GetWindowSurface(gameplay);
             SDL_Rect theGround = {(gameSurface ->w/5 -300), (gameSurface ->h - 50), 1920, 50};
             SDL_Rect platformplacement1 = {(gameSurface ->w/3 -200), (gameSurface ->h - 160), 600, 40};
             SDL_Rect platformplacement2 = {(gameSurface ->w/3 -10), (gameSurface ->h -610), 55, 450};
             SDL_Rect platformplacement3 = {(gameSurface ->w/3 -210), (gameSurface ->h - 610), 450, 40};
             SDL_Rect platformplacement4 = {(gameSurface ->w/5 -300), (gameSurface ->h - 250), 200, 40};
             SDL_Rect platformplacement5 = {(gameSurface ->w/5 -300), (gameSurface ->h - 500), 200, 40};
             SDL_Rect platformplacement6 = {(gameSurface ->w/5 -300), (gameSurface ->h - 850), 1920, 150};
             SDL_Rect platformplacement7 = {(gameSurface ->w/3 -207), (gameSurface ->h - 370), 200, 40};
             SDL_Rect platformplacement8 = {(gameSurface ->w -400), (gameSurface ->h - 450), 400, 60};
             SDL_Rect platformplacement9 = {(gameSurface ->w/2 -186), (gameSurface ->h - 390), 250, 25};
             SDL_Rect platformplacement10 = {(gameSurface ->w/2 -186), (gameSurface ->h - 366), 150, 25};
             SDL_Rect platformplacement11 = {(gameSurface ->w -400), (gameSurface ->h - 260), 300, 40};
             SDL_Rect platformplacement12 = {(gameSurface ->w/2 -50), (gameSurface ->h - 366), 220, 25};
             SDL_Rect platformplacement13 = {(gameSurface ->w -650), (gameSurface ->h - 345), 220, 25};
             SDL_Rect platformplacement14 = {(gameSurface ->w -600), (gameSurface ->h - 325), 220, 25};
             SDL_Rect platformplacement15 = {(gameSurface ->w -550), (gameSurface ->h - 305), 220, 25};
             SDL_Rect platformplacement16 = {(gameSurface ->w -500), (gameSurface ->h - 285), 220, 25};
             SDL_Rect platformplacement17 = {(gameSurface ->w -200), (gameSurface ->h - 200), 300, 50};
             SDL_Rect platformplacement18 = {(gameSurface ->w/2 -160), (gameSurface ->h - 210), 60, 50};
             SDL_Rect platformplacement19 = {(gameSurface ->w/2 +360), (gameSurface ->h - 500), 60, 50};



            SDL_BlitScaled(gameBackground, NULL, gameSurface, NULL);
            SDL_BlitScaled(ground, NULL , gameSurface ,&theGround);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement1);
            SDL_BlitScaled(platform2, NULL , gameSurface ,&platformplacement2);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement3);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement4);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement5);
            SDL_BlitScaled(band, NULL , gameSurface ,&platformplacement6);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement7);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement8);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement9);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement10);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement11);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement12);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement13);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement14);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement15);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement16);
            SDL_BlitScaled(platform1, NULL , gameSurface ,&platformplacement17);
            SDL_BlitScaled(bjrons, NULL , gameSurface ,&platformplacement18);
            SDL_BlitScaled(bjrons, NULL , gameSurface ,&platformplacement19);


            //Update the surface
           SDL_UpdateWindowSurface(gameplay);
        }

    }

    SDL_Delay(4000);

    //Destroy window
    SDL_DestroyWindow(gameplay);
    //Quit SDL subsystems


    SDL_Quit();
}




