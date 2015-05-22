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
#define PLATFORMAMOUNT 17
#define TEXTAMOUNT 4
#define DRINKAMOUNT 3
#define PLAYERCOUNT 6
#endif

int animate(void* info){
    animationInfo* animator = (animationInfo*) info;
    int i, flickerx=0, flickery=0;
    SDL_RendererFlip bflip = SDL_FLIP_NONE;

    animator->window = SDL_CreateWindow("BJORNS THE GAME",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,700,
        0);//SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(animator->window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    playerInfo referenceplayer[PLAYERCOUNT] = {{0}};

    /*Loading and declaration of all images*/
    SDL_Surface* gameBackground = IMG_Load("../Images/game/bar.jpg");
    SDL_Surface* ground = IMG_Load("../Images/game/ground2.png");
    SDL_Surface* platform1 = IMG_Load("../Images/game/platform_hor.png");
    SDL_Surface* platform2 = IMG_Load("../Images/game/platform_ver.png");
    SDL_Surface* band = IMG_Load("../Images/game/band2.png");
    SDL_Surface* bjorns = IMG_Load("../Images/game/bjorndrapare2.png");
    SDL_Surface* bjornsTom = IMG_Load("../Images/game/bjorndraparetom2.png");
    SDL_Surface* ammo = IMG_Load("../Images/game/caps.png");
    SDL_Surface* bullet = IMG_Load("../Images/game/kapsylvertical.png");
    SDL_Surface* textsurface[TEXTAMOUNT];
    SDL_Surface* rip= IMG_Load("../Images/game/rip.png");

    SDL_Surface* playerSurface[6];
    playerSurface[5] = IMG_Load("../Images/game/spriteGreen.png");
    playerSurface[4] = IMG_Load("../Images/game/spriteOrange.png");
    playerSurface[3] = IMG_Load("../Images/game/spritePurple.png");
    playerSurface[2] = IMG_Load("../Images/game/spriteYellow.png");
    playerSurface[1] = IMG_Load("../Images/game/spriteBlue.png");
    playerSurface[0] = IMG_Load("../Images/game/spritePink.png");

    /*Texture declaration*/
    SDL_Renderer* Rend = NULL;
    SDL_Texture* player[6];

    SDL_Texture* bakgroundTexture;
    SDL_Texture* picture[PLATFORMAMOUNT];
    SDL_Texture* bjornDrapare=NULL;
    SDL_Texture* bjornDTom=NULL;
    SDL_Texture* caps=NULL;
    SDL_Texture* myText[TEXTAMOUNT];
    SDL_Texture* bulletTex;
    SDL_Texture* rip_texture=NULL;

    SDL_Rect spriteClips[4];
    SDL_Rect textSprite[6];
    SDL_Rect textRect[5];
    SDL_Rect capsRect[AMMOAMOUNT];

    /* Fill the platforms with colors */
    SDL_FillRect(platform1, NULL, SDL_MapRGB(platform1->format, 200, 190, 200));
    SDL_FillRect(platform2, NULL, SDL_MapRGB(platform2->format, 200, 190, 200));

    SDL_Surface* screen = SDL_GetWindowSurface(animator->window); //get the screen size
    printf("Width: %d, Height: %d\n", screen->w, screen->h);

    Rend=SDL_GetRenderer(animator->window);
    if(!Rend)
        Rend = SDL_CreateRenderer(animator->window, -1, SDL_RENDERER_ACCELERATED); //Create a Render for the window
    if(!Rend)
        printf("Couldn't start the render: %s\n", SDL_GetError());

    for(i=0;i<6;i++){
        printf("Loading playertextures %d!\n", i);
        player[i] = SDL_CreateTextureFromSurface(Rend, playerSurface[i]);
    }

    bakgroundTexture = SDL_CreateTextureFromSurface(Rend,gameBackground); //Load texture with image "bar.jpg" and Rend
    rip_texture = SDL_CreateTextureFromSurface(Rend, rip);

    /*text info*/
    SDL_Color colorT= {170,60,255};
    TTF_Font *font = TTF_OpenFont("../Images/game/StencilStd.ttf", 100);
    if(font == NULL)
    {
        printf("FONT E NULL\n");
    }

    textsurface[0]= TTF_RenderText_Solid(font, "AMMO:", colorT);
    textsurface[1]= TTF_RenderText_Solid(font, "HP:", colorT);
    textsurface[2]= TTF_RenderText_Solid(font, "Drunk:", colorT);
    textsurface[3]= TTF_RenderText_Solid(font, "012345", colorT);

    printf("surfaces laddade i animation\n");
    /*creating texture for all the images and texts */
    picture[0]= SDL_CreateTextureFromSurface(Rend,ground);
    picture[1]= SDL_CreateTextureFromSurface(Rend,band);
    picture[2]= SDL_CreateTextureFromSurface(Rend,platform1);
    picture[3]= SDL_CreateTextureFromSurface(Rend,platform2);


    int loopcounter;
    for (loopcounter = 4; loopcounter < PLATFORMAMOUNT; ++loopcounter)
    {
        picture[loopcounter]= SDL_CreateTextureFromSurface(Rend,platform1);
    }

    caps= SDL_CreateTextureFromSurface(Rend,ammo);
    bjornDrapare=SDL_CreateTextureFromSurface(Rend,bjorns);
    bulletTex = SDL_CreateTextureFromSurface(Rend, bullet);
    bjornDTom=SDL_CreateTextureFromSurface(Rend,bjornsTom);

    myText[0]=SDL_CreateTextureFromSurface(Rend,textsurface[0]);
    myText[1]=SDL_CreateTextureFromSurface(Rend,textsurface[1]);
    myText[2]=SDL_CreateTextureFromSurface(Rend,textsurface[2]);
    myText[3]=SDL_CreateTextureFromSurface(Rend,textsurface[3]);

    printf("texturer laddade från surfaces i animation.c\n");

    /* Free the used surfaces since we won't use them anymore */
    SDL_FreeSurface(gameBackground);
    SDL_FreeSurface(ground);
    SDL_FreeSurface(platform1);
    SDL_FreeSurface(platform2);
    SDL_FreeSurface(band);
    SDL_FreeSurface(bjorns);
    SDL_FreeSurface(ammo);
    SDL_FreeSurface(bullet);
    SDL_FreeSurface(bjornsTom);
    SDL_FreeSurface(rip);
    for(i=0;i<6;i++){
        SDL_FreeSurface(playerSurface[i]);
    }
    for(i=0;i<TEXTAMOUNT;i++){
        SDL_FreeSurface(textsurface[i]);
    }

    printf("surfaces är friade från att ha laddat textures\n");
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
    animator->platforms[3].y = screen->h -450;;
    animator->platforms[3].w = 45;
    animator->platforms[3].h = 300;

    animator->platforms[4].x = screen ->w/3 -115;
    animator->platforms[4].y = screen  ->h - 560;
    animator->platforms[4].w = 350;
    animator->platforms[4].h = 15;

    animator->platforms[5].x = screen->w/3 -460;
    animator->platforms[5].y = screen->h/2 -100;
    animator->platforms[5].w = 250;
    animator->platforms[5].h = 30;

    animator->platforms[6].x = screen->w/3 -460;
    animator->platforms[6].y = screen->h/2 +140;
    animator->platforms[6].w = 150;
    animator->platforms[6].h = 30;

    animator->platforms[7].x = screen ->w/2 +250;
    animator->platforms[7].y = screen->h/2 -100;
    animator->platforms[7].w = 420;
    animator->platforms[7].h = 60;

    animator->platforms[8].x = screen->w/3 -70;
    animator->platforms[8].y = screen -> h/2 -70;
    animator->platforms[8].w = 100;
    animator->platforms[8].h = 30;

    animator->platforms[9].x = screen->w/3 -139;
    animator->platforms[9].y = screen -> h/2 +60;
    animator->platforms[9].w = 170;
    animator->platforms[9].h = 30;

    animator->platforms[10].x = screen ->w/3 +75;
    animator->platforms[10].y = screen -> h/2 -70;
    animator->platforms[10].w = 100;
    animator->platforms[10].h = 30;

    animator->platforms[11].x = screen ->w/3 +230;
    animator->platforms[11].y = screen -> h/2 -180;
    animator->platforms[11].w = 180;
    animator->platforms[11].h = 15;

    animator->platforms[12].x = screen ->w/2 +390;
    animator->platforms[12].y = screen -> h/2 +150;
    animator->platforms[12].w = 300;
    animator->platforms[12].h = 40;

    animator->platforms[13].x = screen ->w/3 +73;
    animator->platforms[13].y = screen -> h/2 +60;
    animator->platforms[13].w = 170;
    animator->platforms[13].h = 30;

    animator->platforms[14].x = screen ->w/2 +300;
    animator->platforms[14].y = screen -> h/2 +200;
    animator->platforms[14].w = 100;
    animator->platforms[14].h = 20;

    animator->platforms[15].x = screen ->w/2 +220;
    animator->platforms[15].y = screen -> h/2 +230;
    animator->platforms[15].w = 100;
    animator->platforms[15].h = 20;

    animator->platforms[16].x = screen ->w/2 +90;
    animator->platforms[16].y = screen -> h -360;
    animator->platforms[16].w = 260;
    animator->platforms[16].h = 20;

    for(i=0;i<AMMOAMOUNT;i++){
        capsRect[i].x = screen->w*0.9+(i*screen->w*0.032);
        capsRect[i].y = screen->h*0.02;//screen ->h - 740;
        capsRect[i].w = screen->w*0.03;
        capsRect[i].h = screen->h*0.046;
    }

    animator->bjornDrapRect[0].x = screen->w/2 -110;
    animator->bjornDrapRect[0].y = screen->h - 215;
    animator->bjornDrapRect[0].w = screen->w*0.03;//60;
    animator->bjornDrapRect[0].h = screen->h*0.046;//50;

    animator->bjornDrapRect[1].x = screen->w/2 +400;
    animator->bjornDrapRect[1].y = screen->h - 492;
    animator->bjornDrapRect[1].w = screen->w*0.03;//60;
    animator->bjornDrapRect[1].h = screen->h*0.046;//50;

    animator->bjornDrapRect[2].x = screen->w/2 -260;
    animator->bjornDrapRect[2].y = screen->h - 472;
    animator->bjornDrapRect[2].w = screen->w*0.03;//60;
    animator->bjornDrapRect[2].h = screen->h*0.046;//50;

    textRect[0].x= screen->w*0.79;//screen ->w/2 +310;
    textRect[0].y= screen->h*0.02;//screen ->h - 740;
    textRect[0].w= screen->w*0.1;//120;
    textRect[0].h= screen->h*0.055;//60;

    textRect[1].x= screen->w*0.02;//screen ->w/2 -660;
    textRect[1].y= screen->h*0.02;//screen ->h - 740;
    textRect[1].w= screen->w*0.06;
    textRect[1].h= screen->h*0.055;

    textRect[2].x= screen->w*0.14;//screen->w/2 -450;
    textRect[2].y= screen->h*0.02;
    textRect[2].w= screen->w*0.11;
    textRect[2].h= screen->h*0.055;

    textRect[3].x= screen->w*0.09;//screen->w/2 -450;
    textRect[3].y= screen->h*0.02;
    textRect[3].w= screen->w*0.02;
    textRect[3].h= screen->h*0.055;

    textRect[4].x= screen->w*0.26;//screen->w/2 -450;
    textRect[4].y= screen->h*0.02;
    textRect[4].w= screen->w*0.02;
    textRect[4].h= screen->h*0.055;

    //size and position for the player
    for(i=0;i<PLAYERCOUNT;i++){
        animator->players[i].pos.y = screen->h;
        animator->players[i].pos.x = screen->w/2+(i*50);
        animator->players[i].pos.h = screen->h*0.11;//120;
        animator->players[i].pos.w = screen->w*0.034;//66;
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

    for(i=0;i<6;i++){
        textSprite[i].x = i*(textsurface[3]->w/6);
        textSprite[i].y = 0;
        textSprite[i].w = (textsurface[3]->w/6);
        textSprite[i].h = textsurface[3]->h;
    }

    for(i=0;i<PLAYERCOUNT;i++){
        referenceplayer[i] = animator->players[i];
    }

    while (!(*(animator->quit))) // while not Esc
    {
        SDL_PumpEvents();
        SDL_RenderClear(Rend); // Clear the entire screen to our selected color/images.
        SDL_RenderCopy(Rend,  bakgroundTexture,NULL,NULL); //view the background on the render "screen"

        for(i=0; i<PLATFORMAMOUNT; i++) //copy all platforms to the render
        {
            SDL_RenderCopy(Rend, picture[i],NULL,&animator->platforms[i]);
        }

        for(i=0; i<12 ;i++){
            if(animator->bullets[i].TTL != 0){
                if(animator->bullets[i].direction<0){
                    bflip = SDL_FLIP_NONE;
                }else
                    bflip = SDL_FLIP_HORIZONTAL;
                SDL_RenderCopyEx(Rend, bulletTex, NULL, &animator->bullets[i].pos, 0, NULL, bflip);
            }
        }

        for(i=0; i<(*(animator->ammo)); i++) //copy all ammo to the render
        {
            SDL_RenderCopy(Rend, caps, NULL, &capsRect[i]);
        }

        for(i=0; i<DRINKAMOUNT; i++) //copy all "bjornDrapare" to the render "the screen"
        {
            if(!is_set(*(animator->powerup), i))
                SDL_RenderCopy(Rend, bjornDTom, NULL, &animator->bjornDrapRect[i]);
            else
                SDL_RenderCopy(Rend, bjornDrapare, NULL, &animator->bjornDrapRect[i]);
        }


        for(i=0; i<3; i++) // copy all text to the render "screen"
        {
            SDL_RenderCopy(Rend, myText[i], NULL, &textRect[i]);
        }

        SDL_RenderCopy(Rend, myText[3], &textSprite[(animator->player->health)], &textRect[3]);
        SDL_RenderCopy(Rend, myText[3], &textSprite[*(animator->drunk)], &textRect[4]);

        //copy all players
        /*for(i=0;i<PLAYERCOUNT;i++){
            if(animator->players[i].health < 1){
                SDL_RenderCopy(Rend, rip_texture, NULL, &animator->players[i].pos);
            }else{
                if(animator->players[i].dir == 1){
                    animator->flip = SDL_FLIP_NONE;
                }else
                    animator->flip = SDL_FLIP_HORIZONTAL;
                SDL_RenderCopyEx(Rend, player[i], &spriteClips[animator->frame], &animator->players[i].pos, 0, NULL, animator->flip);
            }
        }*/

        for(i=0;i<PLAYERCOUNT;i++){
            if(animator->players[i].health < 1){
                SDL_RenderCopy(Rend, rip_texture, NULL, &animator->players[i].pos);
            }else{
                if(animator->players[i].dir == 1){
                    animator->flip = SDL_FLIP_NONE;
                }else
                    animator->flip = SDL_FLIP_HORIZONTAL;
                if(animator->players[i].pos.x != referenceplayer[i].pos.x){
                    if(animator->players[i].pos.x > referenceplayer[i].pos.x){
                        referenceplayer[i].pos.x+=2;
                        flickerx = 1;
                    }else{
                        referenceplayer[i].pos.x-=2;
                        if(flickerx == 1)
                            flickerx=2;
                        else
                            flickerx=0;
                    }
                    if(flickerx == 2){
                        referenceplayer[i].pos.x = animator->players[i].pos.x;
                    }
                }
                if(animator->players[i].pos.y != referenceplayer[i].pos.y){
                    if(animator->players[i].pos.y > referenceplayer[i].pos.y){
                        referenceplayer[i].pos.y+=2;
                        flickery = 1;
                    }else{
                        referenceplayer[i].pos.y-=2;
                        if(flickery == 1)
                            flickery=2;
                        else
                            flickery=0;
                    }
                    if(flickery == 2){
                        referenceplayer[i].pos.y = animator->players[i].pos.y;
                    }
                }
                if(animator->players[i].pos.w != referenceplayer[i].pos.w){
                    referenceplayer[i] = animator->players[i];
                }
                SDL_RenderCopyEx(Rend, player[i], &spriteClips[animator->frame], &referenceplayer[i].pos, 0, NULL, animator->flip);
            }
        }

        // present the result on the render  "the screen"
        SDL_RenderPresent(Rend);
        SDL_Delay(1000/240);
    }



    //Destroy window
    //Quit SDL subsystems

    /*Destroy all textures*/
    for(i=0; i<PLATFORMAMOUNT; i++)
    {
        SDL_DestroyTexture(picture[i]);
    }


    for(i=0; i<TEXTAMOUNT; i++)
    {
        SDL_DestroyTexture(myText[i]);
    }

    SDL_DestroyTexture(caps);
    SDL_DestroyTexture(bjornDrapare);
    SDL_DestroyTexture(bjornDTom);
    SDL_DestroyTexture(rip_texture);
    for(i=0;i<6;i++){
        SDL_DestroyTexture(player[i]);
    }
    SDL_DestroyRenderer(Rend);
    SDL_DestroyWindow(animator->window);

    printf("Animation-thread exiting!\n");

    return 0;
}
