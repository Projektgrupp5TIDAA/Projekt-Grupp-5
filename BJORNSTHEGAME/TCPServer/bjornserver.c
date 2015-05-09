/*
Main server executable
Created 2015-04-16 by Jonathan K�hre and Dani Daryaweesh
Projekt Grupp 5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif
#include <SDL2/SDL_thread.h>
#include "bjornthreads.h"
#include "bjornstack.h"
#include "bjornshared.h"

int main(int argc, char **argv){
    TCPsocket clientsockets[PLAYERCOUNT]={NULL};
    ThreadStack stack = {0, 0, {0}};
    DataStack cstack = {0, {{0}}}, dstack = {0, {{0}}};
    tinfo threadvariables[PLAYERCOUNT];
    pinfo players[PLAYERCOUNT] = {{HEALTH, {0}, {0}}};
    int quit=0, j, i, threadactive[PLAYERCOUNT], maintimer=0, powerup=0, lastpop=0;
    char sendpackage[200];
    nsend namestruct;
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

                /* If there is a message waiting to be handled it will be sent within the lobby */
                if(!(isEmptyStrStack(cstack))){
                    popString(&cstack, sendpackage);
                    printf("STRING IS: %s\n", sendpackage);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientsockets[i] !=NULL)
                            SDLNet_TCP_Send(clientsockets[i], sendpackage, sizeof(sendpackage));
                    }
                    SDL_Delay(200);
                }else SDL_Delay(200);

                /* If the current stack population differs from the previous state
                 the server will send a name update to connected players */
                if(stack.population != lastpop){
                    SDL_Delay(1000);
                    printf("Players changed!\n");
                    for(i=0;i<PLAYERCOUNT;i++){
                        namestruct.ID[i] = i;
                        strcpy(namestruct.names[i], players[i].playername);
                    }
                    memcpy(&sendpackage, &namestruct, sizeof(namestruct));
                    parseChat(sendpackage, -1, strlen(sendpackage));
                    sendpackage[0] = 'N';
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientsockets[i] != NULL)
                            SDLNet_TCP_Send(clientsockets[i], sendpackage, sizeof(sendpackage));
                    }
                    SDL_Delay(1100);
                    sprintf(sendpackage, "T%d", maintimer);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientsockets[i] != NULL)
                            SDLNet_TCP_Send(clientsockets[i], sendpackage, sizeof(sendpackage));
                    }
                    lastpop = stack.population;
                }
            }

            if(!(isFullStack(stack))){
                /* Goes over to the game-loop */
                printf("Game starting in 2 sec.\n");
                SDL_Delay(2000);
                maintimer = GAMELENGTH;
                printf("Sending time sync message.\n");
                sprintf(sendpackage, "T%d", maintimer);
                for(i=0;i<PLAYERCOUNT;i++){
                    SDLNet_TCP_Send(clientsockets[i], sendpackage, sizeof(sendpackage));
                }
                SDL_Delay(200);
                powerup = 0;
            }else{
                printf("No players remaining, exiting.\n");
                maintimer=0;
                powerup = 0;
            }

            /* Keeps the game active as long as there is players connected to the server */
            while(maintimer > 0 && !(isFullStack(stack))){
                /*
                if(!(isEmptyStrStack(dstack))){
                    popString(&dstack, sendpackage);
                    printf("STRING IS: %s\n", sendpackage);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientsockets[i] !=NULL)
                            SDLNet_TCP_Send(clientsockets[i], sendpackage, sizeof(sendpackage));
                    }
                    SDL_Delay(200);
                }else
                // If there is a message waiting to be handled it will be sent within the lobby
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
