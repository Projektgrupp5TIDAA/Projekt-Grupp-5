#include "gameplay.h"
#include "clientthreads.h"
#include "animation.h"
#define AMMOAMOUNT 3
#define PLATFORMAMOUNT 14
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2

int gameplayWindow(ClientInfo* information)
{
    int i, quit=0;
    updaterInfo updater = {&quit, NULL, &(information->socket), {{0, 0, {0, 0, 0, 0}}}};
    animationInfo animator = {0, &quit, NULL, {NULL}, SDL_FLIP_NONE, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}, {{0, 0, 0, 0}}};
    SDL_Thread* updaterThread, *animatorThread;
    playerInfo playerDummy = {0, 0, {0, 0, 0, 0}};
    SDL_Event event;
    bool onPlatform= false;

    //Create a window
    updater.window = SDL_CreateWindow("BJORNS THE GAME",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,600,//1280,800,
        0);//SDL_WINDOW_FULLSCREEN_DESKTOP);
    if(updater.window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Surface* screen = SDL_GetWindowSurface(updater.window);

    animator.window = updater.window;
    for(i=0;i<PLAYERCOUNT;i++){
        animator.players[i] = &(updater.players[i]);
    }

    playerDummy.pos.y = screen->h/2;
    playerDummy.pos.x = screen->w/2;
    playerDummy.pos.h = screen->h*0.11;
    playerDummy.pos.w = screen->w*0.034;

    //updaterThread = SDL_CreateThread(updateHandler, "Updater", (void*)&updater);

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
                       // playerDummy.pos.x -= 3;
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
                        //playerDummy.pos.x += 3;
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
                    case SDLK_SPACE:
                       //TODO JUMP
                        if(onPlatform == true)
                            {
                                playerDummy.pos.y -= SPEEDy;
                                for(i=0; i<PLATFORMAMOUNT; i++)
                                {
                                    if(checkCollision(playerDummy.pos,animator.platforms[i])==true)
                                    {
                                        playerDummy.pos.y +=SPEEDy;
                                    }
                                }

                            }
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