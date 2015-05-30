/*
Main server executable
Created 2015-04-16 by Jonathan K�hre and Dani Daryaweesh
Projekt Grupp 5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
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
    clientHandler clientHandlerData[PLAYERCOUNT] = {{0}};
    playerInfo players[PLAYERCOUNT] = {{5, 0, 0, {0}}};
    playerInfo playersend[PLAYERCOUNT] = {{5, 0, 0, {0}}};
    PollInfo pollerinfo = {0};
    SDL_Thread* connectionpoller, *timerthr;
    TimerInfo timerinfo = {0};
    char sendpackage[PACKETSIZE];
    int i, j, lastpop=0, newdata=0, powerupcheck=0, activeplayers;
    nsend namestruct;
    SDL_mutex* playerArrayMutex = SDL_CreateMutex();
    pollerinfo.chatStack.mutex = SDL_CreateMutex();
    pollerinfo.bulletStack.mutex = SDL_CreateMutex();

    /* Initializing the information for the stack and threads */
    for(i=0;i<PLAYERCOUNT;i++){
        clientHandlerData[i].ID = i;
        clientHandlerData[i].newdata = &newdata;
        clientHandlerData[i].player = &players[i];
        clientHandlerData[i].powerup = &timerinfo.powerup;
        clientHandlerData[i].playerArrayMutex = &playerArrayMutex;
        for(j=0;j<PLAYERCOUNT;j++){
            clientHandlerData[i].names[j] = clientHandlerData[j].playername;
        }
        pushStack(&pollerinfo.clientInformationStack, &clientHandlerData[i]);
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
        if(pollerinfo.clientInformationStack.population < 5){
            printf("Lobby starting in 2 seconds.\n");
            SDL_Delay(2000);
            timerinfo.maintimer = LOBBYLENGTH;

            sprintf(sendpackage, "T%d", timerinfo.maintimer);
            for(i=0;i<PLAYERCOUNT;i++){
                if(clientHandlerData[i].socket != NULL)
                    SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
            }

            /* Keeps the lobby active as long as there is players connected to the server */
            while(timerinfo.maintimer > 0 && (pollerinfo.clientInformationStack.population < 5)){

                /* If there is a message waiting to be handled it will be sent within the lobby */
                if(!(isEmptyStrStack(pollerinfo.chatStack))){
                    if(SDL_LockMutex(pollerinfo.chatStack.mutex) < 0){
                        printf("Main couldn't lock chatmutex: %s\n", SDL_GetError());
                    }else{
                        popString(&pollerinfo.chatStack, sendpackage, sizeof(sendpackage));
                        for(i=0;i<PLAYERCOUNT;i++){
                            if(clientHandlerData[i].socket !=NULL)
                                SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                        }
                        SDL_Delay(200);
                        SDL_UnlockMutex(pollerinfo.chatStack.mutex);
                    }
                }else SDL_Delay(200);

                /* If the current stack population differs from the previous state
                 the server will send a name and time update to connected players */
                if(pollerinfo.clientInformationStack.population != lastpop){
                    SDL_Delay(1000);
                    printf("Players changed!\n");
                    for(i=0;i<PLAYERCOUNT;i++){
                        namestruct.ID[i] = i;
                        strcpy(namestruct.names[i], clientHandlerData[i].playername);
                    }
                    memcpy(&sendpackage, &namestruct, sizeof(namestruct));
                    parseString(sendpackage, -1, strlen(sendpackage));
                    sendpackage[0] = 'N';
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientHandlerData[i].socket != NULL)
                            SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                    }
                    SDL_Delay(1100);
                    sprintf(sendpackage, "T%d", timerinfo.maintimer);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientHandlerData[i].socket != NULL)
                            SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                    }
                    lastpop = pollerinfo.clientInformationStack.population;
                }
            }


            /* If enough players are connected the server starts the game timer and game communications protocols,
               before doing so, the server sends a time-sync message to all connected players */
            if(pollerinfo.clientInformationStack.population < 5){
                for(i=0;i<PLAYERCOUNT;i++){
                    if(clientHandlerData[i].socket != NULL)
                        SDLNet_TCP_Send(clientHandlerData[i].socket, "G", 2);
                }
                printf("Game starting in 4 sec.\n");
                SDL_Delay(4000);
                timerinfo.maintimer = GAMELENGTH;
                printf("Sending time sync message.\n");
                sprintf(sendpackage, "T%d", timerinfo.maintimer);
                for(i=0;i<PLAYERCOUNT;i++){
                    if(clientHandlerData[i].socket != NULL)
                        SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
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
                    if(clientHandlerData[i].socket != NULL)
                        SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                }
            }

            /* Keeps the game active as long as there is players connected to the server */
            while(timerinfo.maintimer > 0 && (pollerinfo.clientInformationStack.population < 5)){
                if(newdata == 1){
                    //printf("Sending player update message!\n");
                    activeplayers = 0;

                    if(SDL_LockMutex(playerArrayMutex) < 0){
                        printf("Main couldn't lock playerArrayMutex: %s\n", SDL_GetError());
                    }else{
                        for(i=0;i<PLAYERCOUNT;i++){
                            if(players[i].pos.x != 0){
                                memcpy(&playersend[activeplayers], &players[i], sizeof(playerInfo));
                                activeplayers++;
                            }
                        }

                        printf("Sending player update with %d active players!\n", activeplayers);

                        makePlayerPacket(sendpackage, playersend, activeplayers);

                        parseString(sendpackage, -1, sizeof(sendpackage));
                        sendpackage[0] = 'P';
                        sendpackage[1] = activeplayers;
                        for(i=0;i<PLAYERCOUNT;i++){
                            if(clientHandlerData[i].socket != NULL)
                                SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, 4*activeplayers + 3);
                        }
                        newdata = 0;
                        SDL_UnlockMutex(playerArrayMutex);
                    }
                }else{
                /* If there is a message waiting to be handled it will be sent as long as no high-priority updates are waiting */
                    if(!(isEmptyStrStack(pollerinfo.chatStack))){
                        if(SDL_LockMutex(pollerinfo.chatStack.mutex) < 0){
                            printf("Main couldn't lock chatmutex: %s\n", SDL_GetError());
                        }else{
                            popString(&pollerinfo.chatStack, sendpackage, sizeof(sendpackage));
                            for(i=0;i<PLAYERCOUNT;i++){
                                if(clientHandlerData[i].socket != NULL)
                                    SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                            }
                            SDL_UnlockMutex(pollerinfo.chatStack.mutex);
                        }
                    }else SDL_Delay(200);
                }

                if(!(isEmptyStrStack(pollerinfo.bulletStack))){
                    if(SDL_LockMutex(pollerinfo.bulletStack.mutex) < 0){
                        printf("Main couldn't lock bulletmutex: %s\n", SDL_GetError());
                    }else{
                        //printf("Sending the bullet position update");
                        popString(&pollerinfo.bulletStack, sendpackage, sizeof(sendpackage));
                        for(i=0;i<PLAYERCOUNT;i++){
                            if(clientHandlerData[i].socket != NULL)
                                SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, 6);
                        }
                        SDL_Delay(15);
                        SDL_UnlockMutex(pollerinfo.bulletStack.mutex);
                    }
                }
                if(timerinfo.powerup != powerupcheck){
                    printf("Sending power update!\n");
                    sprintf(sendpackage, "D%d", timerinfo.powerup);
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientHandlerData[i].socket != NULL)
                            SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                    }
                    powerupcheck = timerinfo.powerup;
                }
            }
            printf("Game stopping.\n");
        }else{
            /* If only one player is connected to the server, the lobby-functions should still remain,
               although useless, you can chat with yourself.. 
               (also serves as a release for the stack not to be overpopulated before being able to send) */
            timerinfo.maintimer = 0;
            if(!(isEmptyStrStack(pollerinfo.chatStack))){
                if(SDL_LockMutex(pollerinfo.chatStack.mutex) < 0){
                        printf("Main couldn't lock chatmutex: %s\n", SDL_GetError());
                }else{
                    popString(&pollerinfo.chatStack, sendpackage, sizeof(sendpackage));
                    for(i=0;i<PLAYERCOUNT;i++){
                        if(clientHandlerData[i].socket != NULL)
                            SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                    }
                    SDL_Delay(200);
                    SDL_UnlockMutex(pollerinfo.chatStack.mutex);
                }
            }else SDL_Delay(200);

            if(pollerinfo.clientInformationStack.population != lastpop){
                SDL_Delay(1000);
                printf("Players changed!\n");
                for(i=0;i<PLAYERCOUNT;i++){
                    namestruct.ID[i] = i;
                    strcpy(namestruct.names[i], clientHandlerData[i].playername);
                }
                memcpy(&sendpackage, &namestruct, sizeof(namestruct));
                parseString(sendpackage, -1, strlen(sendpackage));
                sendpackage[0] = 'N';
                for(i=0;i<PLAYERCOUNT;i++){
                    if(clientHandlerData[i].socket != NULL)
                        SDLNet_TCP_Send(clientHandlerData[i].socket, sendpackage, sizeof(sendpackage)+1);
                }
                lastpop = pollerinfo.clientInformationStack.population;
            }
        }
    }

    SDLNet_Quit();
    return 0;
}