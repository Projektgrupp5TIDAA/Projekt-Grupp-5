#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bjornthreads.h"
#include "bjornstack.h"
#include "bjornshared.h"

int main(int argc, char **argv){
    TCPsocket clientsockets[PLAYERCOUNT];
    ThreadStack stack = {0, 0, {0}};
    DataStack cstack = {0, {0}}, dstack = {0, {0}};
    tinfo threadvariables[PLAYERCOUNT];
    pinfo players[PLAYERCOUNT] = {{HEALTH, {0}, {0}}};
    int quit=0, j, i, threadactive[PLAYERCOUNT], maintimer=0, powerup=0;
    char cmess[40];
    PollInfo pollerinfo;
    TimerInfo timerinfo = {&maintimer, &powerup};
    SDL_Thread* connectionpoller, *timerthr;

    /* Initiates the pointers for the connectionpoller-thread */
    pollerinfo.stack = &stack;
    pollerinfo.cstack = &cstack;
    pollerinfo.dstack = &dstack;
    pollerinfo.quit = &quit;

    /* Initializing the information for the stack and threads */
    for(i=0;i<PLAYERCOUNT;i++){
        threadvariables[i].ID = i;
        threadvariables[i].socket = &clientsockets[i];
        threadvariables[i].player = &players[i];
        threadvariables[i].active = &threadactive[i];
        for(j=0;j<PLAYERCOUNT;j++){
            threadvariables[i].names[j] = players[j].playername;
        }
        pushStack(&stack, &threadvariables[i]);
    }

    /* Initialize SDL */
    if(SDL_Init(SDL_INIT_AUDIO) != 0){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    /* Initialize SDL_net */
    if (SDLNet_Init() != 0){
        fprintf(stderr, "Error initializing SDL_net: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }

    /* Start the connectionpoller-thread */
    connectionpoller = SDL_CreateThread(poller, "ConnectionPoller", (void*)&pollerinfo);
    if(connectionpoller == NULL){
        fprintf(stderr, "Error creating the connection-polling-thread: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Delay(1000);

    /* Start the timehandler-thread */
    timerthr = SDL_CreateThread(timer, "ConnectionPoller", (void*)&timerinfo);
    if(timerthr == NULL){
        fprintf(stderr, "Error creating the timer-thread: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Delay(300);
    /* Main broadcasting loop */
    while(!quit){
        maintimer = 0;
        /* Starts the lobby-timer if a player is connected to the server */
        if(!(isFullStack(stack))){
            printf("Lobby started.\n");
            maintimer = LOBBYLENGTH;

            /* Keeps the lobby active as long as there is players connected to the server */
            while(maintimer > 0 && !(isFullStack(stack))){
                /* Every time the timer hits a number dividable by 10 the server will send a time-syncronization */
                if(!(maintimer%10)){
                    /*for(i=0;i<PLAYERCOUNT;i++){
                        SDLNet_TCP_Send(clientsockets[i], maintimer, sizeof(maintimer));
                    }*/
                    SDL_Delay(1000);
                }
                /* If there is a message waiting to be handled it will be sent within the lobby */
                if(!(isEmptyStrStack(cstack))){
                    strcpy(popString(&cstack), cmess);
                    for(i=0;i<PLAYERCOUNT;i++){
                        SDLNet_TCP_Send(clientsockets[i], cmess, 40);
                    }
                }else SDL_Delay(200);
            }
            SDL_Delay(1000);

            /* Goes over to the game-loop */
            printf("Game starting.\n");
            maintimer = GAMELENGTH;

            /* Keeps the game active as long as there is players connected to the server */
            while(maintimer > 0 && !(isFullStack(stack))){
                /*
                if(!(isEmptyStrStack(dstack))){
                    /*for(i=0;i<PLAYERCOUNT;i++){
                        SDLNet_TCP_Send(clientsockets[i], maintimer, sizeof(maintimer));
                    }
                    SDL_Delay(1000);
                }else
                /* If there is a message waiting to be handled it will be sent within the lobby
                if(!(isEmptyStrStack(cstack))){
                    strcpy(popString(&cstack), cmess);
                    for(i=0;i<PLAYERCOUNT;i++){
                        SDLNet_TCP_Send(clientsockets[i], cmess, 40);
                    }
                }else*/ SDL_Delay(200);
            }
            printf("Game stopping.\n");
        }
    }

    SDLNet_Quit();
    return 0;
}
