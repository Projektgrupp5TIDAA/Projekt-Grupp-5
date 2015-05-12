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
    tinfo threadvariables[PLAYERCOUNT] = {{0, NULL, {5, {0}, {0}}, {NULL}}};
    PollInfo pollerinfo = {0, {0, NULL, {NULL}}, {0, {{0}}}, {0, {{0}}}};
    SDL_Thread* connectionpoller, *timerthr;
    TimerInfo timerinfo = {0, 0};
    char sendpackage[200];
    int i, j, lastpop=0;
    nsend namestruct;

    /* Initializing the information for the stack and threads */
    for(i=0;i<PLAYERCOUNT;i++){
        threadvariables[i].ID = i;
        for(j=0;j<PLAYERCOUNT;j++){
            threadvariables[i].names[j] = threadvariables[j].player.playername;
        }
        pushStack(&pollerinfo.stack, &threadvariables[i]);
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
    while(!(pollerinfo.quit)){
        timerinfo.maintimer = 0;

        /* Starts the lobby-timer if a player is connected to the server */
        if(pollerinfo.stack.population < 5){
            printf("Lobby starting in 2 seconds.\n");
            SDL_Delay(2000);
            timerinfo.maintimer = LOBBYLENGTH;

            sprintf(sendpackage, "T%d", timerinfo.maintimer);
            for(i=0;i<PLAYERCOUNT;i++){
                if(threadvariables[i].socket != NULL)
                    SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
            }

            /* Keeps the lobby active as long as there is players connected to the server */
            while(timerinfo.maintimer > 0 && (pollerinfo.stack.population < 5)){

                /* If there is a message waiting to be handled it will be sent within the lobby */
                if(!(isEmptyStrStack(pollerinfo.cstack))){
                    popString(&pollerinfo.cstack, sendpackage);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(threadvariables[i].socket !=NULL)
                            SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
                    }
                    SDL_Delay(200);
                }else SDL_Delay(200);

                /* If the current stack population differs from the previous state
                 the server will send a name and time update to connected players */
                if(pollerinfo.stack.population != lastpop){
                    SDL_Delay(1000);
                    printf("Players changed!\n");
                    for(i=0;i<PLAYERCOUNT;i++){
                        namestruct.ID[i] = i;
                        strcpy(namestruct.names[i], threadvariables[i].player.playername);
                    }
                    memcpy(&sendpackage, &namestruct, sizeof(namestruct));
                    parseString(sendpackage, -1, strlen(sendpackage));
                    sendpackage[0] = 'N';
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(threadvariables[i].socket != NULL)
                            SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
                    }
                    SDL_Delay(1100);
                    sprintf(sendpackage, "T%d", timerinfo.maintimer);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(threadvariables[i].socket != NULL)
                            SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
                    }
                    lastpop = pollerinfo.stack.population;
                }
            }


            /* If enought players are connected the server starts the game timer and game communications protocols,
               before doing so, the server sends a time-sync message to all connected players */
            if(pollerinfo.stack.population < 5){
                printf("Game starting in 2 sec.\n");
                SDL_Delay(2000);
                timerinfo.maintimer = GAMELENGTH;
                printf("Sending time sync message.\n");
                sprintf(sendpackage, "T%d", timerinfo.maintimer);
                for(i=0;i<PLAYERCOUNT;i++){
                    if(threadvariables[i].socket != NULL)
                        SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
                }
                SDL_Delay(200);
                timerinfo.powerup = 0;
            }else{
                printf("No players remaining, exiting.\n");
                timerinfo.maintimer=0;
                timerinfo.powerup = 0;
                SDL_Delay(1100);
                sprintf(sendpackage, "T%d", timerinfo.maintimer);
                for(i=0;i<PLAYERCOUNT;i++){
                    if(threadvariables[i].socket != NULL)
                        SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
                }
            }

            /* Keeps the game active as long as there is players connected to the server */
            while(timerinfo.maintimer > 0 && (pollerinfo.stack.population < 5)){
                if(!(isEmptyStrStack(pollerinfo.dstack))){
                }else{
                // If there is a message waiting to be handled it will be sent within the lobby
                    if(!(isEmptyStrStack(pollerinfo.cstack))){
                        popString(&pollerinfo.cstack, sendpackage);
                        for(i=0;i<PLAYERCOUNT;i++){
                            SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, 40);
                        }
                    }else SDL_Delay(200);
                }
            }
            printf("Game stopping.\n");
        }else{

            /* If only one player is connected to the server, the lobby-functions should still remain,
               although useless, you can chat with yourself.. 
               (also serves as a release for the stack not to be overpopulated before being able to send) */
            timerinfo.maintimer = 0;
            if(!(isEmptyStrStack(pollerinfo.cstack))){
                    popString(&pollerinfo.cstack, sendpackage);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(threadvariables[i].socket != NULL)
                            SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
                    }
                SDL_Delay(200);
            }else SDL_Delay(200);

            if(pollerinfo.stack.population != lastpop){
                SDL_Delay(1000);
                printf("Players changed!\n");
                for(i=0;i<PLAYERCOUNT;i++){
                    namestruct.ID[i] = i;
                    strcpy(namestruct.names[i], threadvariables[i].player.playername);
                }
                memcpy(&sendpackage, &namestruct, sizeof(namestruct));
                parseString(sendpackage, -1, strlen(sendpackage));
                sendpackage[0] = 'N';
                for(i=0;i<PLAYERCOUNT;i++){
                    if(threadvariables[i].socket != NULL)
                        SDLNet_TCP_Send(threadvariables[i].socket, sendpackage, sizeof(sendpackage));
                }
                lastpop = pollerinfo.stack.population;
            }
        }
    }

    SDLNet_Quit();
    return 0;
}
