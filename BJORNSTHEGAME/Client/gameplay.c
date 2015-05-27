
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
#define PLATFORMAMOUNT 17
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2
#define BULLET_TTL 12
#define SPEEDx 10
#define SPEEDy 200

int gameplayWindow(ClientInfo* information)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_Music* music = Mix_LoadMUS("../Sounds/Music/MovProp.mp3");
    Mix_PlayMusic(music, -1);

    int i, j, quit=0, ammo=AMMOAMOUNT, drunk=0, powerup=0, onground=0;
    animationInfo animator = {0, &quit, &ammo, &drunk, &powerup, NULL, NULL, NULL, {{0}}, SDL_FLIP_NONE, {{{0}}}, {{0}}, {{0}}, &information->names};
    updaterInfo updater = {&quit, &powerup, 0, &(information->socket), NULL};
    timerInfo timer = {&updater.timer, &quit, {NULL}, &animator, &information->socket};
    SDL_Thread* updaterThread, *animatorThread, *timerThread;
    playerInfo playerDummy = {5, 0, 1, {0, 0, 0, 0}};
    bullet bulletDummy = {{0}};
    SDL_Event event;

    /* to animate on the windows and ammo */
    animator.player = &playerDummy;
    animator.gameclock= &updater.timer; 
    animator.names = &information->names;

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
    playerDummy.pos.h = 56;//screen->h*0.08;
    playerDummy.pos.w = 38;//screen->w*0.030;

    bulletDummy.pos.y = 0;
    bulletDummy.pos.x = 0;
    bulletDummy.pos.h = screen->h*0.013;
    bulletDummy.pos.w = screen->w*0.0030;

    while(!quit){
        while(SDL_PollEvent(&event)) //events
        {
        
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
            if(event.type == SDL_KEYDOWN)
            {
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    quit = 1;
                    break;
                }
                if(animator.player->health > 0){
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_LEFT:
                            bulletDummy.direction=-1;
                            playerDummy.pos.x -= SPEEDx;
                            playerDummy.dir = -1;
                            for(i=0; i<PLATFORMAMOUNT; i++)
                            {
                                if(( playerDummy.pos.x < 0 ) || ( playerDummy.pos.x + playerDummy.pos.w >screen ->w ) ||checkCollision(playerDummy.pos,animator.platforms[i])==true)
                                {
                                    playerDummy.pos.x+= SPEEDx;
                                }
                            }

                            for(i=0;i<3;i++)
                                if(checkCollision(playerDummy.pos, animator.bjornDrapRect[i])){
                                    if(is_set(powerup, i) > 0 && drunk < 5){
                                        drunk++;
                                        clr_bit(&powerup, i);
                                        sendPowerUpdate(powerup, &information->socket);
                                    }
                                }

                            //animator.flip = SDL_FLIP_HORIZONTAL;

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
                            playerDummy.dir = 1;
                            for(i=0; i<PLATFORMAMOUNT; i++)
                            {
                                if((playerDummy.pos.x < 0 ) || (playerDummy.pos.x + playerDummy.pos.w > screen->w )||checkCollision(playerDummy.pos,animator.platforms[i])==true)
                                {
                                    playerDummy.pos.x -= SPEEDx;
                                }
                            }

                            for(i=0;i<3;i++)
                                if(checkCollision(playerDummy.pos, animator.bjornDrapRect[i])){
                                    if(is_set(powerup, i) > 0 && drunk < 5){
                                        drunk++;
                                        clr_bit(&powerup, i);
                                        sendPowerUpdate(powerup, &information->socket);
                                    }
                                }

                            //animator.flip = SDL_FLIP_NONE;

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
                                if(playerDummy.dir == 1){
                                    bulletDummy.pos.x = playerDummy.pos.x + playerDummy.pos.w;
                                }else{
                                    bulletDummy.pos.x = playerDummy.pos.x;
                                }
                                bulletDummy.pos.y = playerDummy.pos.y+(playerDummy.pos.h/4);
                                bulletDummy.TTL = BULLET_TTL;
                                bulletDummy.dmg = drunk+1;
                                sendBulletUpdate(bulletDummy, &information->socket);
                                ammo--;
                            }
                            break;
                        case SDLK_r:
                            printf("Reloading\n");
                            ammo =3;
                            break;
                        case SDLK_SPACE:
                            for(i=0;i<PLATFORMAMOUNT;i++)
                                if(checkgravity(playerDummy.pos, animator.platforms[i], 3))
                                    onground = 1;
                            if(onground == 1){
                                for(j=0;j<8;j++){
                                    playerDummy.pos.y -= SPEEDy/8;
                                    for(i=0; i<PLATFORMAMOUNT; i++){
                                        if(checkCollision(playerDummy.pos,animator.platforms[i]))
                                        {
                                            playerDummy.pos.y +=SPEEDy/8;
                                            sendPlayerUpdate(playerDummy, &information->socket);
                                            j=8;
                                        }
                                    }
                                    sendPlayerUpdate(playerDummy, &information->socket);
                                    SDL_Delay(33);
                                }
                            }

                            for(i=0;i<3;i++)
                                if(checkCollision(playerDummy.pos, animator.bjornDrapRect[i])){
                                    if(is_set(powerup, i) > 0 && drunk < 5){
                                        drunk++;
                                        clr_bit(&powerup, i);
                                        sendPowerUpdate(powerup, &information->socket);
                                    }
                                }

                            sendPlayerUpdate(playerDummy, &information->socket);
                            printf("jump!\n");
                            onground = 0;
                            break;
                        default:
                            printf("Wrong key! :D\n");
                            SDL_Delay(8);//For dani
                            break;
                    }
                }
            }
        }
        SDL_Delay(1000/60);
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

int sendPowerUpdate(int powerup, TCPsocket* socket){
    char packet[sizeof(int)+1];
    sprintf(packet, "D%d", powerup);
    printf("Sending %d!\n", powerup);
    if(*socket != NULL){
        SDLNet_TCP_Send(*socket, packet, sizeof(packet)+1);
        return 0;
    }else
        return 1;
}
