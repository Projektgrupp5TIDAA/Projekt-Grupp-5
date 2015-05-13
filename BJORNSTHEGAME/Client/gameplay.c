#include "gameplay.h"
#include "clientthreads.h"
#include "animation.h"

int gameplayWindow(ClientInfo* information)
{
    updaterInfo updater = {NULL, &(information->socket), {{0, 0, {0, 0, 0, 0}}}};
    SDL_Thread* updaterThread, *animator;
    char serializedplayer[sizeof(playerInfo)+2] = {0};
    playerInfo playerDummy = {0, 0, {0, 0, 0, 0}};
    SDL_Event event;
    int i, quit=0;

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
    
    playerDummy.pos.y = screen->h/2;
    playerDummy.pos.x = screen->w/2;
    playerDummy.pos.h = screen->h*0.11;
    playerDummy.pos.w = screen->w*0.034;
    
    updaterThread = SDL_CreateThread(updateHandler, "Updater", (void*)&updater);

    animator = SDL_CreateThread(animate, "Animator", (void*)&updater);

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
                        playerDummy.pos.x -= 3;
                        memcpy(&serializedplayer, &playerDummy, sizeof(playerDummy));
                        parseString(serializedplayer, -1, sizeof(serializedplayer));
                        printf("PlayerDummy x+y = %d, %d\n", playerDummy.pos.x, playerDummy.pos.y);
                        serializedplayer[0] = 'P';
                        SDLNet_TCP_Send(information->socket, serializedplayer, sizeof(serializedplayer));
                        break;
                    case SDLK_RIGHT:
                        playerDummy.pos.x += 3;
                        memcpy(&serializedplayer, &playerDummy, sizeof(playerDummy));
                        parseString(serializedplayer, -1, sizeof(serializedplayer));
                        printf("PlayerDummy x+y = %d, %d\n", playerDummy.pos.x, playerDummy.pos.y);
                        serializedplayer[0] = 'P';
                        SDLNet_TCP_Send(information->socket, serializedplayer, sizeof(serializedplayer));
                        break;
                    case SDLK_SPACE:
                        //TODO: Jump
                        break;
                    default:
                        break;
                }

            }
        }
    }
    
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