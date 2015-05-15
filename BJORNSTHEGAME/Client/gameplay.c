#include "gameplay.h"
#include "clientthreads.h"
#include "animation.h"
#define AMMOAMOUNT 3
#define PLATFORMAMOUNT 14
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2

int gameplayWindow(ClientInfo* information)
{
    int i, quit=0, ammo=3;
    updaterInfo updater = {&quit, NULL, &(information->socket), {{0, 0, {0, 0, 0, 0}}},{{0,0,0,0}},&ammo};
        animationInfo animator = {0, &quit, NULL, {{0,0,{0,0,0,0}}},{{0,0,0,0}},SDL_FLIP_NONE, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}};
    SDL_Thread* updaterThread, *animatorThread;
    playerInfo playerDummy = {0,0,{0, 0, 0, 0}};
    brecv bulletinfo={{0,0,0,0}};
    SDL_Event event;
    bool right=false, left=false, upp=false;

    //Create a window
    updater.window = SDL_CreateWindow("BJORNS THE GAME",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,800,
        0);//SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(updater.window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* screen = SDL_GetWindowSurface(updater.window);
    
    /* to animate on the windows and ammo */ 
    animator.window = updater.window;
    animator.animateammo= &ammo;
    for(i=0;i<PLAYERCOUNT;i++){
        animator.players[i] = &(updater.players[i]);
        animator.bullets[i]= &(updater.bullets[i]);
    }

    playerDummy.pos.y = screen->h/4*3+60;
    playerDummy.pos.x = screen->w/2;
    playerDummy.pos.h = screen->h*0.11;
    playerDummy.pos.w = screen->w*0.034;
    
    bulletinfo.bulletpos.y = screen->h/4*3+90;
    bulletinfo.bulletpos.x = screen->w/2;
    bulletinfo.bulletpos.h = screen->h*0.13;
    bulletinfo.bulletpos.w = screen->w*0.050;

    updaterThread = SDL_CreateThread(updateHandler, "Updater", (void*)&updater);

    animatorThread = SDL_CreateThread(animate, "Animator", (void*)&animator);

    while(!quit){
        while (SDL_PollEvent(&event)) //events
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            if(event.type == SDL_KEYDOWN)
            {
                //Select surfaces based on key press
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_LEFT:
                        left=true;
                        right=false;
                        playerDummy.pos.x -= SPEEDx;
                        /*for(i=0; i<PLATFORMAMOUNT; i++)
                        {
                            if(( playerDummy.pos.x < 0 ) || ( playerDummy.pos.x + playerDummy.pos.w >screen ->w ) ||checkCollision(playerDummy.pos,animator.platforms[i])==true)
                            {
                                playerDummy.pos.x+= SPEEDx;
                            }
                        }*/
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
                        //playerDummy.pos.x += 3;
                        right=true;
                        left=false;
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
                        printf("Spaming shoots\n");
                        ammo--;
                        if(ammo > 0){
                            if(right== true){
                                bulletinfo.bulletpos.x +=SPEEDx;
                                sendBulletUpdate(bulletinfo, &information->socket);
                            }
                            else if(left == true){
                                bulletinfo.bulletpos.x -=SPEEDx;
                                sendBulletUpdate(bulletinfo, &information->socket);
                            }
                        }
                        // reload
                        else{
                            ammo=3;
                            SDL_Delay(500);
                        }
                        break;
                    case SDLK_SPACE:
                        playerDummy.pos.y -= SPEEDy+ 200;
                        for(i=0; i<PLATFORMAMOUNT; i++){
                            if(checkCollision(playerDummy.pos,animator.platforms[i]))
                                    {
                                        playerDummy.pos.y +=GRAVITY;
                                    }
                                }
                        sendPlayerUpdate(playerDummy, &information->socket);
                        playerDummy.pos.y += GRAVITY;
                        for(i=0; i<PLATFORMAMOUNT; i++){
                            if(checkCollision(playerDummy.pos, animator.platforms[i] )){
                                playerDummy.pos.y += GRAVITY;
                            }
                        }
                        sendPlayerUpdate(playerDummy, &information->socket);
                        break;
                    default:
                        break;
                }

            }
        }
    }

    SDL_WaitThread(updaterThread, &i);
    SDL_WaitThread(animatorThread, &i);
    SDL_Quit();
    TTF_Quit();
    return 0;

}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

int sendPlayerUpdate(playerInfo playerDummy, TCPsocket* socket){
    
    char serializedplayer[sizeof(playerInfo)+2] = {0};
    memcpy(&serializedplayer, &playerDummy, sizeof(playerDummy));
    parseString(serializedplayer, -1, sizeof(serializedplayer));
    printf("PlayerDummy x+y = %d, %d\n", playerDummy.pos.x, playerDummy.pos.y);
    serializedplayer[0] = 'P';
    if(*socket != NULL){
        SDLNet_TCP_Send(*socket, serializedplayer, sizeof(serializedplayer));
        return 0;
    }else
        return 1;
}

int sendBulletUpdate(brecv bulletsinfo, TCPsocket* socket){
    char bulletstmp[sizeof(brecv)+2] = {0};
    memcpy(&bulletstmp, &bulletsinfo, sizeof(bulletsinfo));
    parseString(bulletstmp, -1, sizeof(bulletstmp));
    printf("bullets x+y = %d, %d\n", bulletsinfo.bulletpos.x, bulletsinfo.bulletpos.y);
    bulletstmp[0]= 'B';
    if(*socket != NULL){
        SDLNet_TCP_Send(*socket, bulletstmp, sizeof(bulletstmp));
        return 0;
    }else
        return 1;
}