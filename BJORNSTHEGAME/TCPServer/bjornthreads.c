#include <stdio.h>
#include <string.h>
#include "bjornthreads.h"
#include "bjornshared.h"

/* Thread execution function */
SDL_ThreadFunction *Handler(void* thr){
    TCPsocket intermediary;
    char packet[PACKETSIZE];
    HandlerInfo* thread = (HandlerInfo *) thr;
    printf("Thread is active!\n");

    /* Sets the intermediary socket to handle the request */
    intermediary = SDLNet_TCP_Accept((*(thread->socket)));

    /* Recieves the request */
    SDLNet_TCP_Recv(intermediary, packet, PACKETSIZE);

    /* Handles the incoming request depending on type */
    switch(packet[0]){
        case 'I':
            /* If the incoming request is an information-probing request the server will send the necessary information */
            printf("Information request recieved, sending.\n");
            if(isEmptyStack(*(thread->stack))){
                SDLNet_TCP_Send(intermediary, "F", 1);
            }else{
                sprintf(packet, "%s - %d/%d", SERVERNAME, (PLAYERCOUNT - (thread->stack->population)), PLAYERCOUNT);
                SDLNet_TCP_Send(intermediary, packet, PACKETSIZE);
                SDLNet_TCP_Close(intermediary);
                printf("Information sent, now exiting thread!\n");
            }
            return 0;
        case 'C':
            /* If the incoming request is a connection-request the server will, if possible assign an open slot */
            printf("Connection request recieved, assigning.\n");
            if(isEmptyStack(*(thread->stack)))
                SDLNet_TCP_Send(intermediary, "F", 1);
            else{
                tinfo* clientvar = popStack(thread->stack);
                *(clientvar->socket) = intermediary;
                SDLNet_TCP_Close(intermediary);
            }
            return 0;
        default:
            /* If the request is not recognized the server will return error */
            SDLNet_TCP_Send(intermediary, "ERROR: Bad request.", 40);
            return 1;
    }




    /* Main loop of the thread, only run if the thread is set to active
    while(1){
        if((*(*thread).active) == 1){
        // Exit the thread if the program is stopped
        if((*(*thread).quit) == 1){
            SDLNet_TCP_Send((*(*thread).socket), "SERVERSHUTDOWN", 14);
            return 0;
        }

        // Set the name at the start of connection
        if(name == 0){
            if(SDLNet_TCP_Recv(*((*thread).socket), (*thread).player.playername, 20))
            printf("Player named: %s connected!\n", (*thread).player.playername);
            name = 1;
            SDLNet_TCP_Recv(*((*thread).socket), incoming, PACKETSIZE);
        }else{

        // If a packet is recieved, check if for the flags or if it's a end of connection transmission
        if(SDLNet_TCP_Recv(*((*thread).socket), incoming, PACKETSIZE)){
            if(strstr("EXITCONNECTION", incoming)){
                printf("Thread #%d is now inactive!\n", (*thread).threadID);
                pushSocketStack((*thread).servants, (*thread).threadID); // pushes sockets id to the stack
                *((*thread).active) = 0;
                name = 0;
            }else

            // Parse and post if the flag is set to chat
            if(incoming[0] == 'C'){
                parseChat(incoming, 1, strlen(incoming));
                if(strstr("tapir", incoming)){
                //(*thread).player.drunkLevel = 4;
                printf("Codeword detected! Drunklevel of %s set to Phöz.\n", (*thread).player.playername);
                }else{
                    printf("%s says: %s\n", (*thread).player.playername, incoming);
                }
            }

            // Functions for data will be placed here
            if(incoming[0] == 'D'){
                printf("Data recieved.\n");
            }
        }
    }
    }else SDL_Delay(200); //If this isn't here the server will constantly take all available CPU-power unless otherwise limited.
  }*/
}

SDL_ThreadFunction* poller(void* information){
    PollInfo* info = (PollInfo*)information;
    IPaddress listenerIP;
    TCPsocket socket;
    SDLNet_SocketSet activity = SDLNet_AllocSocketSet(1);
    HandlerInfo connectionhandler = {info->quit, &socket, info->stack};

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

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply){
    ply->health = HEALTH;
    ply->position[0] = 5;
    ply->position[1] = 5;
}
