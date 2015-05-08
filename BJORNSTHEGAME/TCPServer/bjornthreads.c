/*
Functions required by the server to execute threads
Created 2015-04-16 by Jonathan Kåhre
Edited by Dani Daryaweesh
Projekt Grupp 5
*/

#include <stdio.h>
#include <string.h>
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif
#include <SDL2/SDL_thread.h>
#include "bjornthreads.h"
#include "bjornshared.h"

/* Thread execution function */
int Handler(void* thr){
    TCPsocket socket;
    char packet[PACKETSIZE], serialnames[sizeof(nsend)];
    int i;
    nsend names;
    tinfo* clientvar;
    HandlerInfo* thread = (HandlerInfo *) thr;
    printf("Thread is active!\n");

    /* Accepts the incoming connection on the socket to handle the request */
    socket = SDLNet_TCP_Accept((*(thread->socket)));

    /* Recieves the request */
    SDLNet_TCP_Recv(socket, packet, PACKETSIZE);

    /* Handles the incoming request depending on type */
    switch(packet[0]){
        case 'I':
            /* If the incoming request is an information-probing request the server will send the necessary information */
            printf("Information request recieved, sending.\n");
            if(isEmptyStack(*(thread->stack))){
                SDLNet_TCP_Send(socket, "F", 1);
            }else{
                sprintf(packet, "%s - %d/%d", SERVERNAME, (PLAYERCOUNT - (thread->stack->population)), PLAYERCOUNT);
                SDLNet_TCP_Send(socket, packet, PACKETSIZE);
                SDLNet_TCP_Close(socket);
                printf("Information sent, now exiting thread!\n");
            }
            return 0;
        case 'C':
            /* If the incoming request is a connection-request the server will, if possible assign an open slot */
            printf("Connection request recieved, assigning.\n");
            if(isEmptyStack(*(thread->stack)))
                SDLNet_TCP_Send(socket, "F", 1);
            else{
                clientvar = popStack(thread->stack);
                *(clientvar->socket) = socket;
            }
            break;
        default:
            /* If the request is not recognized the server will return error */
            printf("ERROR: Bad request recieved.\n");
            SDLNet_TCP_Send(socket, "ERROR: Bad request.", 40);
            SDLNet_TCP_Close(socket);
            return 1;
    }

    /* Gets the name from the client */
    while(1){
        if(SDLNet_TCP_Recv(socket, clientvar->player->playername, 20)){
            break;
        }
    }

    printf("Player %s was assigned to thread %d.\n", clientvar->player->playername, clientvar->ID);

    /* The main thread-loop */
    while(1){
        /*
            If anything is recieved from the socket it will be handled differently depending on what type of message it is, it can be 1 of 5:
            1. A data-message with the message-prefix 'D' which is put on the data-stack and will be redistributed to the clients.
            2. A player-update-message with the message-prefix 'P' which is handled internally by the thread by directly updating the player-data.
            3. A chat-message with the message-prefix 'C' which is put on the chat-stack and will be redistributed to the clients.(With less priority than Data)
            4. A name-request-message with the message-prefix 'N' which is immidiately responded to with all the active names on the server in the form of a serialized struct.
            5. A EXITCONNECTION message, which quits the thread and closes the connection.
        */
        if(SDLNet_TCP_Recv(socket, packet, PACKETSIZE)){
            if((strstr(packet, "EXITCONNECTION"))){
                printf("Exit command recieved, quitting thread #%d!\n", clientvar->ID);
                memset(clientvar->player->playername,0,strlen(clientvar->player->playername));
                SDLNet_TCP_Close(socket);
                clientvar->socket = NULL;
                pushStack(thread->stack, clientvar);
                return 0;
            }else{
                switch(packet[0]){
                    case 'D':
                        printf("Data recieved, pushing to stack!\n");
                        pushString(thread->dstack, packet);
                        break;
                    case 'P':
                        printf("Playerupdate recieved, updating!\n");
                        //updatePlayer(packet, clientvar->player);
                        break;
                    case 'C':
                        printf("Chat message recieved, pushing to stack!\n");
                        pushString(thread->cstack, packet);
                        break;
                    case 'N':
                        printf("Name request recieved, sending!\n");
                        for(i=0;i<PLAYERCOUNT;i++){
                            names.ID[i] = i;
                            strcpy(names.names[i], clientvar->names[i]);
                        }
                        memcpy(&serialnames, &names, sizeof(names));
                        SDLNet_TCP_Send(socket, serialnames, sizeof(serialnames));
                        break;
                }
            }
        }
    }
}

/* Thread-function which purpose is to handle the incoming connections and assign them to threads */
int poller(void* information){
    PollInfo* info = (PollInfo*)information;
    IPaddress listenerIP;
    TCPsocket socket;
    SDLNet_SocketSet activity = SDLNet_AllocSocketSet(1);
    HandlerInfo connectionhandler = {info->quit, &socket, info->stack, info->cstack, info->dstack};

    /* Resolve listener ip */
    if(SDLNet_ResolveHost(&listenerIP,NULL,PORT) < 0){
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return 1;
    }

    /* Open the socket and add it to the socketset */
    if (!(socket = SDLNet_TCP_Open(&listenerIP))){
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return 1;
    }
    SDLNet_AddSocket(activity, socket);

    printf("Connection-poller thread is now active, awaiting incoming connections.\n");

    /* Main connection assignment loop */
    while(!*(info->quit)){
        while(1){
            /* Whenever there is activity on the socket a thread is spawned to handle it */
            if(SDLNet_CheckSockets(activity, 50) > 0){
                printf("Activity found, starting thread!\n");
                SDL_DetachThread(SDL_CreateThread(Handler, "Thread", (void*)&connectionhandler));
                SDL_Delay(100);
                break;
            }else SDL_Delay(200);
        }
    }
    return 0;
}

/* Thread-function which main function is to handle all the timed events */
int timer(void* information){
    TimerInfo* info = (TimerInfo*) information;
    short timerset[10], i;
    printf("Timer thread running!\n");
    SDL_Delay(1000);
    for(i=0;i<10;i++)
        timerset[i] = GAMELENGTH+1;
    while(1){
        /* Thread will only ever do anything if the main-timer is > 0 */
        if(*(info->main) > 0){
            /* The powerup-timer, checks all the powerups to see if they are active or not, if they are inactive
               they will be assigned a time on the game-timer that will trigger them once the timer hits that value */
            printf("Ticking. Timer: %d, Powerups: %d\n", *(info->main), *(info->powerup));
            for(i=0;i<10;i++){
                if(!(is_set((*(info->powerup)), i+1))){
                    if(*(info->main) == timerset[i]){
                        set_bit(info->powerup, i);
                    }else if(timerset[i] > *(info->main)){
                        if(*(info->main) -POWERTIMER > 0){
                            timerset[i] = *(info->main)-POWERTIMER;
                        }else{
                            timerset[i] = 0;
                        }
                    }
                }
            }

            /* And then the game-timer ticks down */
            (*(info->main))--;
        }
        SDL_Delay(1000);
    }
    return 0;
}
