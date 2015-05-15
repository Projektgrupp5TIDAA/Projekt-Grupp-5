#include "gameplay.h"
#include "clientthreads.h"
#include "animation.h"
#define AMMOAMOUNT 3
#define PLATFORMAMOUNT 14
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2
#define BULLET_TTL 12

int gameplayWindow(ClientInfo* information)
{
    int i, quit=0, ammo=AMMOAMOUNT;
    animationInfo animator = {0, &quit, &ammo, NULL, {{0, 0, {0, 0, 0, 0}}}, SDL_FLIP_NONE, {{{0,0,0,0}, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}};
    updaterInfo updater = {&quit, 0, NULL, &(information->socket), NULL};
    timerInfo timer = {&updater.timer, &quit, {NULL}};
    SDL_Thread* updaterThread, *animatorThread, *timerThread;
    playerInfo playerDummy = {0, 0, {0, 0, 0, 0}};
    bullet bulletDummy = {{0,0,0,0}, 0, 0, 0};
    SDL_Event event;

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

    updater.players = &(animator.players[0]);

    for(i=0;i<12;i++){
        updater.bullets[i] = &(animator.bullets[i]);
        timer.bullets[i] = &(animator.bullets[i]);
    }

    playerDummy.pos.y = screen->h/4*3+60;
    playerDummy.pos.x = screen->w/2;
    playerDummy.pos.h = screen->h*0.11;
    playerDummy.pos.w = screen->w*0.034;
    
    bulletDummy.pos.y = 0;
    bulletDummy.pos.x = 0;
    bulletDummy.pos.h = screen->h*0.013;
    bulletDummy.pos.w = screen->w*0.0030;

    updaterThread = SDL_CreateThread(updateHandler, "Updater", (void*)&updater);

    animatorThread = SDL_CreateThread(animate, "Animator", (void*)&animator);

    timerThread = SDL_CreateThread(timeupdater, "Timer", (void*)&timer);

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
                        bulletDummy.direction=-1;
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
    SDL_WaitThread(timerThread, &i);
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

int sendBulletUpdate(bullet bulletDummy, TCPsocket* socket){
    char serializedbullet[sizeof(bullet)+2] = {0};
    memcpy(&serializedbullet, &bulletDummy, sizeof(bulletDummy));
    parseString(serializedbullet, -1, sizeof(serializedbullet));
    printf("bullets x+y = %d, %d\n", bulletDummy.pos.x, bulletDummy.pos.y);
    serializedbullet[0]= 'B';
    if(*socket != NULL){
        SDLNet_TCP_Send(*socket, serializedbullet, sizeof(serializedbullet));
        return 0;
    }else
        return 1;
}