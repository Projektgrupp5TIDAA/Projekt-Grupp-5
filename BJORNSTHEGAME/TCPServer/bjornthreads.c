#include <stdio.h>
#include <string.h>
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
            SDLNet_TCP_Send(socket, "ERROR: Bad request.", 40);
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
            If anything is recieved from the socket it will be handled differently depending on what type of message it is, it can be 1 of 3:
            1. A data-message with the message-prefix 'D' which is put on the data-stack and will be redistributed to the clients.
            2. A player-update-message with the message-prefix 'P' which is handled internally by the thread by directly updating the player-data.
            3. A chat-message with the message-prefix 'C' which is put on the chat-stack and will be redistributed to the clients.(With less priority than Data)
            4. A name-request-message with the message-prefix 'N' which is immidiately responded to with all the active names on the server.
        */
        if(SDLNet_TCP_Recv(socket, packet, PACKETSIZE)){
            if((strstr(packet, "EXITCONNECTION"))){
                printf("Exit command recieved, quitting thread %d!\n", clientvar->ID);
                memset(clientvar->player->playername,0,strlen(clientvar->player->playername));
                pushStack(thread->stack, clientvar);
                return 0;
            }else{
                switch(packet[0]){
                    case 'D':
                        pushString(thread->dstack, packet);
                        break;
                    case 'P':
                        //updatePlayer(packet, clientvar->player);
                        break;
                    case 'C':
                        parseChat(packet, 1, strlen(packet));
                        pushString(thread->cstack, packet);
                        break;
                    /* player name request */
                    case 'N':
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
            }
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
            (*(info->main))--;
        }//else
        //printf("Timer is 0, no tick.\n");
        SDL_Delay(1000);
    }
    return 0;
}

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply){
    ply->health = HEALTH;
    ply->position[0] = 5;
    ply->position[1] = 5;
}
