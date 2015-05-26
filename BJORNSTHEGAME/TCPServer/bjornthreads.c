/*
Functions required by the server to execute threads
Created 2015-04-16 by Jonathan Kåhre
Edited by Dani Daryaweesh
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
#include "bjornshared.h"

/* Connection handler function, creates a thread to handle an incoming connection */
int Handler(void* thr){
    HandlerInfo* thread = (HandlerInfo *) thr;
    TCPsocket socket;
    char packet[PACKETSIZE], serialnames[sizeof(nsend)];
    int i, errorcount=0;
    nsend names;
    tinfo* clientvar;
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
                SDLNet_TCP_Send(socket, "F", 2);
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
                SDLNet_TCP_Send(socket, "F", 2);
            else{
                clientvar = popStack(thread->stack);
                clientvar->socket = socket;
            }
            break;
        default:
            /* If the request is not recognized the server will return error */
            printf("ERROR: Bad request recieved.\n");
            SDLNet_TCP_Send(socket, "ERROR: Bad request.", 20);
            SDLNet_TCP_Close(socket);
            return 1;
    }

    /* Gets the name from the client */
    SDLNet_TCP_Send(socket, "A", 2);
    SDLNet_TCP_Recv(socket, clientvar->playername, 20);

    printf("Player %s was assigned to thread %d.\n", clientvar->playername, clientvar->ID);

    /* The main thread-loop */
    while(1){
        /*
            If anything is recieved from the socket it will be handled differently depending on what type of message it is, it can be 1 of 5:
            1. A data-message with the message-prefix 'D' which is put on the data-stack and will be redistributed to the clients.
            2. A player-update-message with the message-prefix 'P' which is handled internally by the thread by directly updating the player-data.
            3. A chat-message with the message-prefix 'C' which is put on the chat-stack and will be redistributed to the clients.(With less priority than Data)
            4. A name-request-message with the message-prefix 'N' which is immidiately responded to with all the active names on the server in the form of a serialized struct.
            5. A powerup-update prefixed with 'D' which updates the active powerups.
            6. An EXITCONNECTION message, which quits the thread and closes the connection.
        */
        emptyString(packet, sizeof(packet));
        if(SDLNet_TCP_Recv(socket, packet, PACKETSIZE)){
            if((strstr(packet, "EXITCONNECTION"))){
                printf("Exit command recieved, quitting thread #%d!\n", clientvar->ID);
                memset(clientvar->playername,0,strlen(clientvar->playername));
                SDLNet_TCP_Close(clientvar->socket);
                pushStack(thread->stack, clientvar);
                return 0;
            }else{
                switch(packet[0]){
                    case 'B':
                        //printf("Bullet data recieved, pushing to stack!\n");
                        parseString(packet, -1, sizeof(packet));
                        packet[0] = 'B';
                        packet[1] = clientvar->ID;
                        pushString(thread->dstack, packet, sizeof(packet));
                        break;
                    case 'P':
                        //printf("Player data recieved, updating!\n");
                        parseString(packet, 1, sizeof(packet));
                        memcpy(clientvar->player, &packet, sizeof(pinfo));
                        //printf("Player data: %d, %d\n", clientvar->player->pos.x, clientvar->player->pos.y);
                        *(clientvar->newdata)=1;
                        break;
                    case 'C':
                        //printf("Chat message recieved, pushing to stack!\n");
                        pushString(thread->cstack, packet, sizeof(packet));
                        break;
                    case 'N':
                        //printf("Name request recieved, sending!\n");
                        for(i=0;i<PLAYERCOUNT;i++){
                            names.ID[i] = i;
                            strcpy(names.names[i], clientvar->names[i]);
                        }
                        memcpy(&serialnames, &names, sizeof(names));
                        SDLNet_TCP_Send(socket, serialnames, sizeof(serialnames)+1);
                        break;
                    case 'D':
                        parseString(packet, 1, sizeof(packet));
                        *(clientvar->powerup) = atoi(packet);
                        printf("Recieved %d!\n", *(clientvar->powerup));
                        break;
                    default:
                        if(++errorcount == 30){ 
                            printf("Too many errors recieved, quitting thread #%d!\n", clientvar->ID);
                            memset(clientvar->playername,0,strlen(clientvar->playername));
                            SDLNet_TCP_Close(clientvar->socket);
                            pushStack(thread->stack, clientvar);
                            return 0;
                        }
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
    HandlerInfo connectionhandler = {&(info->quit), &socket, &(info->stack), &(info->cstack), &(info->dstack)};

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
    while(!(info->quit)){
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
    for(i=0;i<3;i++)
        timerset[i] = GAMELENGTH+1;
    while(1){

        /* Thread will only ever do anything if the main-timer is > 0 */
        if((info->maintimer) > 0){

            /* The powerup-timer, checks all the powerups to see if they are active or not, if they are inactive
               they will be assigned a time on the game-timer that will trigger them once the timer hits that value */
            //printf("Ticking. Timer: %d, Powerups: %d\n", (info->maintimer), info->powerup);
            for(i=0;i<3;i++){
                if(!(is_set(info->powerup, i+1))){
                    if((info->maintimer) == timerset[i]){
                        set_bit(&(info->powerup), i);
                    }else if(timerset[i] > (info->maintimer)){
                        if((info->maintimer) -POWERTIMER > 0){
                            timerset[i] = (info->maintimer)-POWERTIMER;
                        }else{
                            timerset[i] = 0;
                        }
                    }
                }
            }

            /* And then the game-timer ticks down */
            ((info->maintimer))--;
        }else{
            for(i=0;i<3;i++)
                if(timerset[i] < GAMELENGTH+1)
                    timerset[i] = GAMELENGTH+1;
        }
        SDL_Delay(1000);
    }
    return 0;
}
