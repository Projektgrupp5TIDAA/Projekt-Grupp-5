#ifndef _ANIMATION_
#define _ANIMATION_
#ifdef __APPLE__
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#endif
//ALL PLATFORMS
#include <stdio.h>
#include <SDL2/SDL.h>
#include "clientthreads.h"
#include "animation.h"
#include "gameplay.h"
#define AMMOAMOUNT 3
#define PLATFORMAMOUNT 14
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2
#endif

int animate(void* info){
    animationInfo* animator = (animationInfo*) info;
    int i, quit=0;

    /*Loading and declaration of all images*/
    SDL_Surface* gameBackground = IMG_Load("../Images/game/bar.jpg");;
    SDL_Surface* ground = IMG_Load("../Images/game/ground2.png");
    SDL_Surface* platform1 = IMG_Load("../Images/game/platform_hor.png");
    SDL_Surface* platform2 = IMG_Load("../Images/game/platform_ver.png");
    SDL_Surface* band = IMG_Load("../Images/game/band2.png");
    SDL_Surface* playerSurface = IMG_Load("../Images/game/spriteBlue.PNG");
    SDL_Surface* bjorns = IMG_Load("../Images/game/bjorndrapare.png");
    SDL_Surface* ammo = IMG_Load("../Images/game/caps.png");
    SDL_Surface* textsurface[TEXTAMOUNT];

    /*Texture declaration*/
    SDL_Renderer* Renderer = NULL;
    SDL_Texture* player = NULL;
    SDL_Texture* bakgroundTexture;
    SDL_Texture* picture[PLATFORMAMOUNT];
    SDL_Texture* bjornDrapare[DRINKAMOUNT];
    SDL_Texture* caps[AMMOAMOUNT];
    SDL_Texture* myText[TEXTAMOUNT];

    SDL_Rect spriteClips[4];

    /* Fill the platforms with colors */
    SDL_FillRect(platform1, NULL, SDL_MapRGB(platform1->format, 200, 190, 200));
    SDL_FillRect(platform2, NULL, SDL_MapRGB(platform2->format, 200, 190, 200));

    SDL_Surface* screen = SDL_GetWindowSurface(animator->window); //get the screen size
    printf("Width: %d, Height: %d\n", screen->w, screen->h);

    Renderer=SDL_GetRenderer(animator->window);
    if(!Renderer)
        Renderer = SDL_CreateRenderer(animator->window, -1, SDL_RENDERER_ACCELERATED); //Create a Render for the window
    if(!Renderer)
        printf("Couldn't start the render: %s\n", SDL_GetError());
    
    bakgroundTexture = SDL_CreateTextureFromSurface(Renderer,gameBackground); //Load texture with image "bar.jpg" and Renderer
    
    /*text*/
    SDL_Color colorT= {170,60,255};
    TTF_Font *font = TTF_OpenFont("../Images/game/StencilStd.ttf", 100);
    if(font == NULL)
    {
        printf("FONT E NULL\n");
    }

    textsurface[0]= TTF_RenderText_Solid(font, "AMMO:", colorT);
    textsurface[1]= TTF_RenderText_Solid(font, "HP:", colorT);
    textsurface[2]= TTF_RenderText_Solid(font, "Drunk:", colorT);

    player = SDL_CreateTextureFromSurface(Renderer, playerSurface); //the texture of the player
    if(! player)
    {
        printf("Couldnt create texture from surface: %s\n", SDL_GetError());
    }
    /*creating texture for all the images and texts */
    picture[0]= SDL_CreateTextureFromSurface(Renderer,ground );
    picture[1]= SDL_CreateTextureFromSurface(Renderer,band);
    picture[2]= SDL_CreateTextureFromSurface(Renderer,platform1);
    picture[3]= SDL_CreateTextureFromSurface(Renderer,platform2);
    
    int loopcounter;
    for (loopcounter = 4; loopcounter < PLATFORMAMOUNT; ++loopcounter)
    {
        picture[loopcounter]= SDL_CreateTextureFromSurface(Renderer,platform1);
    }

    caps[0]= SDL_CreateTextureFromSurface(Renderer,ammo);
    caps[1]= SDL_CreateTextureFromSurface(Renderer,ammo);
    caps[2]= SDL_CreateTextureFromSurface(Renderer,ammo);

    bjornDrapare[0]=SDL_CreateTextureFromSurface(Renderer,bjorns);
    bjornDrapare[1]=SDL_CreateTextureFromSurface(Renderer,bjorns);

    myText[0]=SDL_CreateTextureFromSurface(Renderer,textsurface[0]);
    myText[1]=SDL_CreateTextureFromSurface(Renderer,textsurface[1]);
    myText[2]=SDL_CreateTextureFromSurface(Renderer,textsurface[2]);

    /*set position for every platform on screen*/
    animator->platforms[0].x = 0;
    animator->platforms[0].y = screen->h-(screen->h*0.06);
    animator->platforms[0].w = screen->w;
    animator->platforms[0].h = screen->h*0.06;

    animator->platforms[1].x = 0;
    animator->platforms[1].y = 0;
    animator->platforms[1].w = screen->w;
    animator->platforms[1].h = screen->h*0.08;

    animator->platforms[2].x = screen->w/3 -200;
    animator->platforms[2].y = screen->h - 180;
    animator->platforms[2].w = 550;
    animator->platforms[2].h = 40;

    animator->platforms[3].x = screen->w/3 +30;
    animator->platforms[3].y = screen->h -590;
    animator->platforms[3].w = 45;
    animator->platforms[3].h = 410;

    animator->platforms[4].x = screen ->w/3 -130;
    animator->platforms[4].y = screen  ->h - 610;
    animator->platforms[4].w = 350;
    animator->platforms[4].h = 40;

    animator->platforms[5].x = screen->w/3 -460;
    animator->platforms[5].y = screen->h/2 -100;
    animator->platforms[5].w = 150;
    animator->platforms[5].h = 30;

    animator->platforms[6].x = screen->w/3 -460;
    animator->platforms[6].y = screen->h/2 +140;
    animator->platforms[6].w = 150;
    animator->platforms[6].h = 30;

    animator->platforms[7].x = screen ->w/2 +300;
    animator->platforms[7].y = screen->h/2 -80;
    animator->platforms[7].w = 400;
    animator->platforms[7].h = 60;

    animator->platforms[8].x = screen->w/3 -139;
    animator->platforms[8].y = screen -> h/2 -70;
    animator->platforms[8].w = 170;
    animator->platforms[8].h = 30;

    animator->platforms[9].x = screen->w/3 -139;
    animator->platforms[9].y = screen -> h/2 +90;
    animator->platforms[9].w = 170;
    animator->platforms[9].h = 30;

    animator->platforms[10].x = screen ->w/3 +75;
    animator->platforms[10].y = screen -> h/2 -70;
    animator->platforms[10].w = 170;
    animator->platforms[10].h = 30;

    animator->platforms[11].x = screen ->w/3 +300;
    animator->platforms[11].y = screen -> h/2 -140;
    animator->platforms[11].w = 170;
    animator->platforms[11].h = 30;

    animator->platforms[12].x = screen ->w/2 +390;
    animator->platforms[12].y = screen -> h/2 +150;
    animator->platforms[12].w = 300;
    animator->platforms[12].h = 40;

    animator->platforms[13].x = screen ->w/3 +73;
    animator->platforms[13].y = screen -> h/2 +30;
    animator->platforms[13].w = 170;
    animator->platforms[13].h = 30;


    for(i=0;i<AMMOAMOUNT;i++){
        animator->capsRect[i].x = screen->w*0.9+(i*screen->w*0.032);
        animator->capsRect[i].y = screen->h*0.02;//screen ->h - 740;
        animator->capsRect[i].w = screen->w*0.03;
        animator->capsRect[i].h = screen->h*0.046;
    }

    animator->bjornDrapare[0].x = screen->w/2 -110;
    animator->bjornDrapare[0].y = screen->h - 228;
    animator->bjornDrapare[0].w = screen->w*0.03;//60;
    animator->bjornDrapare[0].h = screen->h*0.046;//50;

    animator->bjornDrapare[1].x = screen  ->w/2 +400;
    animator->bjornDrapare[1].y = screen ->h - 512;
    animator->bjornDrapare[1].w = screen->w*0.03;//60;
    animator->bjornDrapare[1].h = screen->h*0.046;//50;

    animator->textRect[0].x= screen->w*0.79;//screen ->w/2 +310;
    animator->textRect[0].y= screen->h*0.02;//screen ->h - 740;
    animator->textRect[0].w= screen->w*0.1;//120;
    animator->textRect[0].h= screen->h*0.055;//60;

    animator->textRect[1].x= screen->w*0.02;//screen ->w/2 -660;
    animator->textRect[1].y= screen->h*0.02;//screen ->h - 740;
    animator->textRect[1].w= screen->w*0.06;
    animator->textRect[1].h= screen->h*0.055;

    animator->textRect[2].x= screen->w*0.14;//screen->w/2 -450;
    animator->textRect[2].y= screen->h*0.02;
    animator->textRect[2].w= screen->w*0.11;
    animator->textRect[2].h= screen->h*0.055;

    //size and position for the player
    for(i=0;i<PLAYERCOUNT;i++){
        animator->players[i]->pos.y = screen->h/2;
        animator->players[i]->pos.x = screen->w/2+(i*50);
        animator->players[i]->pos.h = screen->h*0.11;//120;
        animator->players[i]->pos.w = screen->w*0.034;//66;
    }

    /*position of the sprites in the image*/
    spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 210;
    spriteClips[0].h = 348;

    spriteClips[1].x = 210;
    spriteClips[1].y = 0;
    spriteClips[1].w = 210;
    spriteClips[1].h = 348;

    spriteClips[2].x = 420;
    spriteClips[2].y = 0;
    spriteClips[2].w = 210;
    spriteClips[2].h = 348;

    spriteClips[3].x = 630;
    spriteClips[3].y = 0;
    spriteClips[3].w = 210;
    spriteClips[3].h = 348;


    while (!quit) // while not Esc
    {
        SDL_RenderClear(Renderer); // Clear the entire screen to our selected color/images.
        SDL_RenderCopy(Renderer,  bakgroundTexture,NULL,NULL); //view the background on the render "screen"

        for(i=0; i<PLATFORMAMOUNT; i++) //copy all platforms to the render
        {

            SDL_RenderCopy(Renderer, picture[i],NULL,&animator->platforms[i]);
        }

        for(i=0; i<AMMOAMOUNT; i++) //copy all ammo to the render
        {
            SDL_RenderCopy(Renderer, caps[i],NULL,&animator->capsRect[i]);
        }

        for(i=0; i<DRINKAMOUNT; i++) //copy all "bjornDrapare" to the render "the screen"
        {
            SDL_RenderCopy(Renderer, bjornDrapare[i],NULL,&animator->bjornDrapare[i]);
        }
        for(i=0; i<AMMOAMOUNT; i++) // copy all text to the render "screen"
        {
            SDL_RenderCopy(Renderer, myText[i],NULL,&animator->textRect[i]);
        }

        //copy all players
        for(i=0;i<PLAYERCOUNT;i++){
            SDL_RenderCopyEx(Renderer, player, &spriteClips[animator->frame],&animator->players[i]->pos, 0, NULL, animator->flip);
        }

        // present the result on the render  "the screen"
        SDL_RenderPresent(Renderer);
    }



    //Destroy window
    SDL_DestroyWindow(animator->window);
    //Quit SDL subsystems

    /*Destroy all textures*/
    for(i=0; i<PLATFORMAMOUNT; i++)
    {
        SDL_DestroyTexture(picture[i]);
    }

    for(i=0; i<AMMOAMOUNT; i++)
    {
        SDL_DestroyTexture(caps[i]);
    }

    for(i=0; i<DRINKAMOUNT; i++)
    {
        SDL_DestroyTexture(bjornDrapare[i]);
    }

    for(i=0; i<TEXTAMOUNT; i++)
    {
        SDL_DestroyTexture(myText[i]);
    }


    SDL_DestroyTexture(player);
    //  SDL_DestroyWindow(bakgroundTexture);
    SDL_DestroyRenderer(Renderer);
    return 0;
}

/* gravity fucntion 
void moveUP(int* test, SDL_Rect wall, SDL_Surface* s){
    
    float my_gravity= 0.2, MaxFall_speed= 0.5, MyJump_force=5.0, CurJump_force=0.0, Delta_time;
    
    CurJump_force=MyJump_force;
    *(test)+=CurJump_force * Delta_time;
    if(CurJump_force > MaxFall_speed){
        MyJump_force-= my_gravity * Delta_time;
    }else{
        CurJump_force=MaxFall_speed;
    }
}*/