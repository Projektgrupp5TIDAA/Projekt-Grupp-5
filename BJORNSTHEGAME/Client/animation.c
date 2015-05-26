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

int animate(void* info)
{
    animationInfo* animator = (animationInfo*) info;
    int i, flickerx=0, flickery=0;
    SDL_RendererFlip bflip = SDL_FLIP_NONE;
    char *buffer[6]= {NULL};
    char c[sizeof(int)];

    //Create the window
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

    /* Declaration and initialization/loading of all images */
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
    SDL_Surface* scoreboard= IMG_Load("../Images/game/deathboard.png");
    SDL_Surface* deathsurface[PLAYERCOUNT];
    SDL_Surface* scoreSurface[PLAYERCOUNT];

    SDL_Surface* playerSurface[6];
    playerSurface[5] = IMG_Load("../Images/game/spriteGreen.png");
    playerSurface[4] = IMG_Load("../Images/game/spriteOrange.png");
    playerSurface[3] = IMG_Load("../Images/game/spritePurple.png");
    playerSurface[2] = IMG_Load("../Images/game/spriteYellow.png");
    playerSurface[1] = IMG_Load("../Images/game/spriteBlue.png");
    playerSurface[0] = IMG_Load("../Images/game/spritePink.png");

    /* Texture declaration */
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
    SDL_Texture* score_texture= NULL;
    SDL_Texture* deathText[PLAYERCOUNT];
    SDL_Texture* death[PLAYERCOUNT];

    SDL_Rect spriteClips[4];
    SDL_Rect textSprite[6];
    SDL_Rect textRect[5];
    SDL_Rect capsRect[AMMOAMOUNT];
    SDL_Rect scores;
    SDL_Rect scorepos[PLAYERCOUNT];
    SDL_Rect Namepos[PLAYERCOUNT];

    /* Fill the platforms with colors */
    SDL_FillRect(platform1, NULL, SDL_MapRGB(platform1->format, 200, 190, 200));
    SDL_FillRect(platform2, NULL, SDL_MapRGB(platform2->format, 200, 190, 200));

    SDL_Surface* screen = SDL_GetWindowSurface(animator->window); //get the screen size
    printf("Width: %d, Height: %d\n", screen->w, screen->h);

    //Create/Get the renderer from the window depending on if it's running on mac or not
    Rend=SDL_GetRenderer(animator->window);
    if(!Rend)
        Rend = SDL_CreateRenderer(animator->window, -1, SDL_RENDERER_ACCELERATED);
    if(!Rend)
        printf("Couldn't start the render: %s\n", SDL_GetError());

    for(i=0; i<6; i++)
    {
        printf("Loading playertextures %d!\n", i);
        player[i] = SDL_CreateTextureFromSurface(Rend, playerSurface[i]);
    }

    bakgroundTexture = SDL_CreateTextureFromSurface(Rend,gameBackground); //Load texture with image "bar.jpg" and Rend
    rip_texture = SDL_CreateTextureFromSurface(Rend, rip);


    // Load the font into the program
    SDL_Color colorT= {170,60,255};
    TTF_Font *font = TTF_OpenFont("../Images/game/StencilStd.ttf", 35);
    if(font == NULL)
    {
        printf("Font returned a NULL value.\n");
    }

    // Render all of the text surfaces
    textsurface[0]= TTF_RenderText_Blended(font, "AMMO:", colorT);
    textsurface[1]= TTF_RenderText_Blended(font, "HP:", colorT);
    textsurface[2]= TTF_RenderText_Blended(font, "Drunk:", colorT);
    textsurface[3]= TTF_RenderText_Blended(font, "012345", colorT);

    printf("All surfaces loaded.\n");

    /* Creating textures from all of the loaded textures */
    picture[0]= SDL_CreateTextureFromSurface(Rend,ground);
    picture[1]= SDL_CreateTextureFromSurface(Rend,band);
    picture[2]= SDL_CreateTextureFromSurface(Rend,platform1);
    picture[3]= SDL_CreateTextureFromSurface(Rend,platform2);

    score_texture= SDL_CreateTextureFromSurface(Rend, scoreboard);

    for(i=4; i<PLATFORMAMOUNT; ++i)
    {
        picture[i]= SDL_CreateTextureFromSurface(Rend,platform1);
    }

    caps= SDL_CreateTextureFromSurface(Rend,ammo);
    bjornDrapare=SDL_CreateTextureFromSurface(Rend,bjorns);
    bulletTex = SDL_CreateTextureFromSurface(Rend, bullet);
    bjornDTom=SDL_CreateTextureFromSurface(Rend,bjornsTom);

    myText[0]=SDL_CreateTextureFromSurface(Rend,textsurface[0]);
    myText[1]=SDL_CreateTextureFromSurface(Rend,textsurface[1]);
    myText[2]=SDL_CreateTextureFromSurface(Rend,textsurface[2]);
    myText[3]=SDL_CreateTextureFromSurface(Rend,textsurface[3]);

    SDL_Color black= {0,0,0};

    for(i=0; i<PLAYERCOUNT; i++)
    {
        printf("loading deathsurface %d",i);
        //     deathsurface[i]= TTF_RenderText_Blended(font,animator->test->names.names[5-i], black);
        deathsurface[i]= TTF_RenderText_Blended(font,"PHOZ", black);
    }
    for(i=0; i<PLAYERCOUNT; i++)
    {
        deathText[i]=SDL_CreateTextureFromSurface(Rend,deathsurface[i]);
    }


    for(i=0; i<PLAYERCOUNT; i++)
    {
        //itoa(animator->players[i].deaths,buffer,10);
        snprintf(c, sizeof(int), "%d", animator->players[i].deaths);
        buffer[i] = malloc(sizeof(c));
        strcpy(buffer[i], c);

    }
    for(i=0; i<PLAYERCOUNT; i++)
    {

        scoreSurface[i]=TTF_RenderText_Blended(font,buffer[i],black);


    }

    for(i=0; i<PLAYERCOUNT; i++)
    {
        if(death[i]==NULL)
        {
            printf("deth[i] fel");
        }
        death[i]=SDL_CreateTextureFromSurface(Rend,scoreSurface[i]);
    }


    printf("All textures loaded.\n");

    /* Set the positions for every platform on screen */
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

    for(i=0; i<AMMOAMOUNT; i++)
    {
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
    textRect[0].w= textsurface[0]->w;//screen->w*0.1;//120;
    textRect[0].h= textsurface[0]->h;//screen->h*0.055;//60;

    textRect[1].x= screen->w*0.02;//screen ->w/2 -660;
    textRect[1].y= screen->h*0.02;//screen ->h - 740;
    textRect[1].w= textsurface[1]->w;//screen->w*0.06;
    textRect[1].h= textsurface[1]->h;//screen->h*0.055;

    textRect[2].x= screen->w*0.14;//screen->w/2 -450;
    textRect[2].y= screen->h*0.02;
    textRect[2].w= textsurface[2]->w;//screen->w*0.11;
    textRect[2].h= textsurface[2]->h;//screen->h*0.055;

    textRect[3].x= screen->w*0.09;//screen->w/2 -450;
    textRect[3].y= screen->h*0.02;
    textRect[3].w= textsurface[3]->w/6;//screen->w*0.02;
    textRect[3].h= textsurface[3]->h;//screen->h*0.055;

    textRect[4].x= screen->w*0.26;//screen->w/2 -450;
    textRect[4].y= screen->h*0.02;
    textRect[4].w= textsurface[3]->w/6;//screen->w*0.02;
    textRect[4].h= textsurface[3]->h;//screen->h*0.055;

    scores.x= screen->w/3- 85;
    scores.y= screen->h/3;
    scores.h= screen->h/2;
    scores.w= screen->w/2;

    Namepos[0].x= screen->w/2 -240;
    Namepos[0].y= screen->h/2 +20;
    Namepos[0].h= deathsurface[0]->h/2;
    Namepos[0].w= deathsurface[0]->w;

    Namepos[1].x= screen->w/2 -240;
    Namepos[1].y= screen->h/2 +45;
    Namepos[1].h= deathsurface[1]->h/2;
    Namepos[1].w= deathsurface[1]->w;

    Namepos[2].x= screen->w/2 -240;
    Namepos[2].y= screen->h/2 +70;
    Namepos[2].h= deathsurface[2]->h/2;
    Namepos[2].w= deathsurface[2]->w;

    Namepos[3].x= screen->w/2 -240;
    Namepos[3].y= screen->h/2 +95;
    Namepos[3].h= deathsurface[3]->h/2;
    Namepos[3].w= deathsurface[3]->w;

    Namepos[4].x= screen->w/2 -240;
    Namepos[4].y= screen->h/2 +120;
    Namepos[4].h= deathsurface[4]->h/2;
    Namepos[4].w= deathsurface[4]->w;

    Namepos[5].x= screen->w/2 -240;
    Namepos[5].y= screen->h/2 +145;
    Namepos[5].h= deathsurface[5]->h/2;
    Namepos[5].w= deathsurface[5]->w;

    scorepos[0].x= screen->w/2 +200;
    scorepos[0].y= screen->h/2 +20;
    scorepos[0].h= scoreSurface[0]->h/2;
    scorepos[0].w= scoreSurface[0]->w;

    scorepos[1].x= screen->w/2 +200;
    scorepos[1].y= screen->h/2 +45;
    scorepos[1].h= scoreSurface[1]->h/2;
    scorepos[1].w= scoreSurface[1]->w;

    scorepos[2].x= screen->w/2 +200;
    scorepos[2].y= screen->h/2 +70;
    scorepos[2].h= scoreSurface[2]->h/2;
    scorepos[2].w= scoreSurface[2]->w;

    scorepos[3].x= screen->w/2 +200;
    scorepos[3].y= screen->h/2 +95;
    scorepos[3].h= scoreSurface[3]->h/2;
    scorepos[3].w= scoreSurface[3]->w;

    scorepos[4].x= screen->w/2 +200;
    scorepos[4].y= screen->h/2 +120;
    scorepos[4].h= scoreSurface[4]->h/2;
    scorepos[4].w= scoreSurface[4]->w;

    scorepos[5].x= screen->w/2 +200;
    scorepos[5].y= screen->h/2 +145;
    scorepos[5].h= scoreSurface[5]->h/2;
    scorepos[5].w= scoreSurface[5]->w;

    // Size and position for the player
    for(i=0; i<PLAYERCOUNT; i++)
    {
        animator->players[i].pos.y = screen->h;
        animator->players[i].pos.x = screen->w/2+(i*50);
        animator->players[i].pos.h = screen->h*0.11;//120;
        animator->players[i].pos.w = screen->w*0.034;//66;
    }

    /* Positions in the player-sprites */
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

    for(i=0; i<6; i++)
    {
        textSprite[i].x = i*(textsurface[3]->w/6);
        textSprite[i].y = 0;
        textSprite[i].w = (textsurface[3]->w/6);
        textSprite[i].h = textsurface[3]->h;
    }

    for(i=0; i<PLAYERCOUNT; i++)
    {
        referenceplayer[i] = animator->players[i];
    }

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
    SDL_FreeSurface(scoreboard);
    for(i=0; i<6; i++)
    {
        SDL_FreeSurface(playerSurface[i]);
    }
    for(i=0; i<TEXTAMOUNT; i++)
    {
        SDL_FreeSurface(textsurface[i]);
    }
    for(i=0; i<PLAYERCOUNT; i++)
    {   SDL_FreeSurface(deathsurface[i]);
        SDL_FreeSurface(scoreSurface[i]);
    }

    printf("Surfaces have been unallocated.\n");

    while (!(*(animator->quit))) //While the program is running (main animation loop)
    {
        SDL_PumpEvents(); //Since the event-handler is coupled with the video-rendering this is necessary for reading movements in the main thread for some reason
        SDL_RenderClear(Rend); // Clear the entire screen to our selected color/images.
        SDL_RenderCopy(Rend, bakgroundTexture, NULL, NULL); //Render the background onto the buffer of the renderer

        //Render all of the platforms onto the buffer of the renderer
        for(i=0; i<PLATFORMAMOUNT; i++) //copy all platforms to the render
        {
            SDL_RenderCopy(Rend, picture[i],NULL,&animator->platforms[i]);
        }

        //Render all of the active bullets on the screen to the buffer of the renderer
        for(i=0; i<12 ; i++)
        {
            if(animator->bullets[i].TTL != 0)
            {
                if(animator->bullets[i].direction<0)
                {
                    bflip = SDL_FLIP_NONE;
                }
                else
                    bflip = SDL_FLIP_HORIZONTAL;
                SDL_RenderCopyEx(Rend, bulletTex, NULL, &animator->bullets[i].pos, 0, NULL, bflip);
            }
        }

        //Render all of the ammunition-images onto the buffer of the renderer
        for(i=0; i<(*(animator->ammo)); i++) //copy all ammo to the render
        {
            SDL_RenderCopy(Rend, caps, NULL, &capsRect[i]);
        }

        //Render the powerups onto the buffer of the renderer
        for(i=0; i<DRINKAMOUNT; i++)
        {
            if(!is_set(*(animator->powerup), i))
                SDL_RenderCopy(Rend, bjornDTom, NULL, &animator->bjornDrapRect[i]);
            else
                SDL_RenderCopy(Rend, bjornDrapare, NULL, &animator->bjornDrapRect[i]);
        }

        //Render the fixed text (AMMO: HP:) onto the buffer of the renderer
        for(i=0; i<3; i++)
        {
            SDL_RenderCopy(Rend, myText[i], NULL, &textRect[i]);
        }

        //Render the health and drunklevel text onto the buffer of the renderer
        SDL_RenderCopy(Rend, myText[3], &textSprite[(animator->player->health)], &textRect[3]);
        SDL_RenderCopy(Rend, myText[3], &textSprite[*(animator->drunk)], &textRect[4]);

        //Render the players onto the buffer of the renderer aswell as manage the movement of the reference
        for(i=0; i<PLAYERCOUNT; i++)
        {
            if(animator->players[i].health < 1)
            {
                SDL_RenderCopy(Rend, rip_texture, NULL, &animator->players[i].pos);
            }
            else
            {
                if(animator->players[i].dir == 1)
                {
                    animator->flip = SDL_FLIP_NONE;
                }
                else
                    animator->flip = SDL_FLIP_HORIZONTAL;
                if(animator->players[i].pos.x != referenceplayer[i].pos.x)
                {
                    if(animator->players[i].pos.x > referenceplayer[i].pos.x)
                    {
                        referenceplayer[i].pos.x+=2;
                        flickerx = 1;
                    }
                    else
                    {
                        referenceplayer[i].pos.x-=2;
                        if(flickerx == 1)
                            flickerx=2;
                        else
                            flickerx=0;
                    }
                    if(flickerx == 2)
                    {
                        referenceplayer[i].pos.x = animator->players[i].pos.x;
                    }
                }
                if(animator->players[i].pos.y != referenceplayer[i].pos.y)
                {
                    if(animator->players[i].pos.y > referenceplayer[i].pos.y)
                    {
                        referenceplayer[i].pos.y+=2;
                        flickery = 1;
                    }
                    else
                    {
                        referenceplayer[i].pos.y-=2;
                        if(flickery == 1)
                            flickery=2;
                        else
                            flickery=0;
                    }
                    if(flickery == 2)
                    {
                        referenceplayer[i].pos.y = animator->players[i].pos.y;
                    }
                }
                if(animator->players[i].pos.w != referenceplayer[i].pos.w)
                {
                    referenceplayer[i] = animator->players[i];
                }
                SDL_RenderCopyEx(Rend, player[i], &spriteClips[animator->frame], &referenceplayer[i].pos, 0, NULL, animator->flip);
            }
        }

        //Render the present buffer of the renderer onto the screen
        SDL_RenderPresent(Rend);

        SDL_Delay(1000/240);
    }

    if(*(animator->gameclock) == 0)
    {

        SDL_RenderCopy(Rend, score_texture, NULL, &scores);

         for(i=0; i<PLAYERCOUNT; i++)
        {
            SDL_RenderCopy(Rend, deathText[i], NULL, &Namepos[i]);
        }

        for(i=0; i<PLAYERCOUNT; i++)
        {

            SDL_RenderCopy(Rend,death[i],NULL,&scorepos[i]);

        }
        SDL_RenderPresent(Rend);
        SDL_Delay(10000);
    }

    /* Destroy all textures, the window and the renderer */
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
    SDL_DestroyTexture(score_texture);
    for(i=0; i<6; i++)
    {
        SDL_DestroyTexture(player[i]);
    }

    for(i=0; i<PLAYERCOUNT; i++)
    {
        SDL_DestroyTexture(deathText[i]);

    }

    for(i=0; i<PLAYERCOUNT; i++)
    {

        SDL_DestroyTexture(death[i]);
    }
    SDL_DestroyRenderer(Rend);
    SDL_DestroyWindow(animator->window);

    printf("Animation-thread exiting!\n");

    return 0;
}
