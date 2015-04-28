#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bjornthreads.h"
#include "bjornstack.h"
#include "bjornshared.h"

int main(int argc, char **argv){
    TCPsocket clientsockets[PLAYERCOUNT];
    ThreadStack stack = {0,{0}};
    tinfo threadvariables[PLAYERCOUNT];
    pinfo players[PLAYERCOUNT] = {{HEALTH, {0}, {0}}};
    PollInfo pollerinfo;
    int quit=0, i;
    SDL_Thread* connectionpoller;

    pollerinfo.stack = &stack;
    pollerinfo.quit = &quit;

    for(i=0;i<PLAYERCOUNT;i++){
        threadvariables[i].ID = i;
        threadvariables[i].socket = &clientsockets[i];
        threadvariables[i].player = &players[i];
        pushStack(&stack, &threadvariables[i]);
    }

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* Initialize SDL_net */
    if (SDLNet_Init() != 0){
        fprintf(stderr, "Error initializing SDL_net: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    connectionpoller = SDL_CreateThread(poller, "ConnectionPoller", (void*)&pollerinfo);
    if(connectionpoller == NULL){
        fprintf(stderr, "Error creating the connection-polling-thread: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    while(!quit){
        SDL_Delay(200);
    }

    SDLNet_Quit();
    return 0;
}

