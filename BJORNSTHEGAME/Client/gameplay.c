
#include <stdio.h>
#include <SDL2/SDL.h>
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
#include "bjornstartup.h"
#include "lobby.h"
#include "clientthreads.h"
#include "gameplay.h"
#include "animation.h"
#define AMMOAMOUNT 3
#define PLATFORMAMOUNT 14
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2
#define BULLET_TTL 12

int gameplayWindow(ClientInfo* information)
{
    int i, quit=0, ammo=AMMOAMOUNT, drunk=0;
    animationInfo animator = {0, &quit, &ammo, &drunk, NULL, NULL, {{0, 0, {0, 0, 0, 0}}}, SDL_FLIP_NONE, {{{0,0,0,0}, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}};
    updaterInfo updater = {&quit, 0, &(information->socket), NULL};
    timerInfo timer = {&updater.timer, &quit, {NULL}, &animator, &information->socket};
    SDL_Thread* updaterThread, *animatorThread, *timerThread;
    playerInfo playerDummy = {5, 0, {0, 0, 0, 0}};
    bullet bulletDummy = {{0,0,0,0}, 0, 0, 0};
    SDL_Event event;

    SDL_Init(SDL_INIT_EVERYTHING);

    /* to animate on the windows and ammo */
    animator.player = &playerDummy;

    updater.players = &(animator.players[0]);

    for(i=0;i<12;i++){
        updater.bullets[i] = &(animator.bullets[i]);
        timer.bullets[i] = &(animator.bullets[i]);
    }


    animatorThread = SDL_CreateThread(animate, "Animator", (void*)&animator);
    SDL_Delay(500);
    SDL_Surface* screen = SDL_GetWindowSurface(animator.window);
    updaterThread = SDL_CreateThread(updateHandler, "Updater", (void*)&updater);
    timerThread = SDL_CreateThread(timeupdater, "Timer", (void*)&timer);

    playerDummy.pos.y = screen->h/4*3+60;
    playerDummy.pos.x = screen->w/2;
    playerDummy.pos.h = screen->h*0.08;
    playerDummy.pos.w = screen->w*0.030;

    bulletDummy.pos.y = 0;
    bulletDummy.pos.x = 0;
    bulletDummy.pos.h = screen->h*0.013;
    bulletDummy.pos.w = screen->w*0.0030;
    int kolla = 0;
    while(!quit){
        
        while(SDL_PollEvent(&event)) //events 
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }else

            if(event.type == SDL_KEYDOWN)
            {
                //Select surfaces based on key press
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        quit = 1;
                        break;
                    case SDLK_LEFT:
                        bulletDummy.direction=-1;
                        playerDummy.pos.x -= SPEEDx;
                        for(i=0; i<PLATFORMAMOUNT; i++)
                        {
                            if(( playerDummy.pos.x < 0 ) || ( playerDummy.pos.x + playerDummy.pos.w >screen ->w ) ||checkCollision(playerDummy.pos,animator.platforms[i])==true)
                            {
                                playerDummy.pos.x+= SPEEDx;
                            }
                        }
                        animator.flip = SDL_FLIP_HORIZONTAL;

                        if(animator.frame == 2)
                        {
                            animator.frame = 0;
                        }
                        else if(animator.frame == 0)
                        {
                            animator.frame = 3;
                        }
                        else if(animator.frame == 3)
                        {
                            animator.frame = 1;
                        }
                        else
                        {
                            animator.frame = 2;
                        }
                        sendPlayerUpdate(playerDummy, &information->socket);
                        break;

                    case SDLK_RIGHT:
                        bulletDummy.direction = 1;
                        playerDummy.pos.x += SPEEDx;
                        for(i=0; i<PLATFORMAMOUNT; i++)
                        {
                            if((playerDummy.pos.x < 0 ) || (playerDummy.pos.x + playerDummy.pos.w >screen ->w )||checkCollision(playerDummy.pos,animator.platforms[i])==true)
                            {

                                playerDummy.pos.x -= SPEEDx;

                            }
                        }
                        animator.flip = SDL_FLIP_NONE;

                        if(animator.frame == 2)
                        {
                            animator.frame = 0;
                        }
                        else if(animator.frame == 0)
                        {
                            animator.frame = 3;
                        }
                        else if(animator.frame == 3)
                        {
                            animator.frame = 1;
                        }
                        else
                        {
                            animator.frame = 2;
                        }
                        sendPlayerUpdate(playerDummy, &information->socket);
                        break;

                    case SDLK_x:
                        printf("Shooting!\n");
                        if(ammo > 0){
                            bulletDummy.pos.x = playerDummy.pos.x;
                            bulletDummy.pos.y = playerDummy.pos.y+(playerDummy.pos.h/4);
                            bulletDummy.TTL = BULLET_TTL;
                            sendBulletUpdate(bulletDummy, &information->socket);
                            ammo--;
                        }else{
                            ammo=3;
                        }
                        break;
                    case SDLK_SPACE:
                       playerDummy.pos.y -= SPEEDy;
                        for(i=0; i<PLATFORMAMOUNT; i++){
                            if(checkCollision(playerDummy.pos,animator.platforms[i]))
                            {
                                playerDummy.pos.y +=SPEEDy;
                            }
                        }
                        sendPlayerUpdate(playerDummy, &information->socket);
                        SDL_Delay(500);
                    /*    playerDummy.pos.y += GRAVITY;
                        for(i=0; i<PLATFORMAMOUNT; i++){
                            if(checkCollision(playerDummy.pos, animator.platforms[i] )== true){
                                playerDummy.pos.y += GRAVITY;
                            }
                        }
                        sendPlayerUpdate(playerDummy, &information->socket);*/
                        printf("jump!\n");
                        break;
                    default:
                        printf("Wrong key! :D\n");
                        SDL_Delay(8);//For dani
                        break;
                }
            }else printf("Hejsan, hit kommer man om man drar musen!\n");
        }
        SDL_Delay(1000/60);
        SDL_PumpEvents();
        SDL_FlushEvent(SDL_KEYDOWN);
        SDL_FlushEvent(SDL_KEYUP);
    }

    SDL_WaitThread(updaterThread, &i);
    SDL_WaitThread(animatorThread, &i);
    SDL_WaitThread(timerThread, &i);
    return 0;

}

/* Function to check collisions between rectangles, inspired by the rectangle collision from Lazyfoo */
int checkCollision(SDL_Rect a, SDL_Rect b)
{
    //If any of the sides from A are outside of B
    if((a.y + a.h) <= b.y)
    {
        return 0;
    }

    if((b.y + b.h) <= a.y)
    {
        return 0;
    }

    if((a.x+a.w) <= b.x)
    {
        return 0;
    }

    if((b.x+b.w) <= a.x)
    {
        return 0;
    }

    //If none of the sides from A are outside B
    return 1;
}

int sendPlayerUpdate(playerInfo playerDummy, TCPsocket* socket){
    char serializedplayer[sizeof(playerInfo)+2] = {0};
    memcpy(&serializedplayer, &playerDummy, sizeof(playerDummy));
    parseString(serializedplayer, -1, sizeof(serializedplayer));
    //printf("PlayerDummy x+y = %d, %d\n", playerDummy.pos.x, playerDummy.pos.y);
    serializedplayer[0] = 'P';
    if(*socket != NULL){
        SDLNet_TCP_Send(*socket, serializedplayer, sizeof(serializedplayer));
        return 0;
    }else
        return 1;
}

int sendBulletUpdate(bullet bulletDummy, TCPsocket* socket){
    char serializedbullet[sizeof(bullet)+2] = {0};
    memcpy(&serializedbullet, &bulletDummy, sizeof(bulletDummy));
    parseString(serializedbullet, -1, sizeof(serializedbullet));
    //printf("bullets x+y = %d, %d\n", bulletDummy.pos.x, bulletDummy.pos.y);
    serializedbullet[0]= 'B';
    if(*socket != NULL){
        SDLNet_TCP_Send(*socket, serializedbullet, sizeof(serializedbullet));
        return 0;
    }else
        return 1;
}
