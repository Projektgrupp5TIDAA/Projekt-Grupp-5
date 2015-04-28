#include <stdio.h>
#include <string.h>
#include "bjornthreads.h"
#include "bjornshared.h"

/* Thread execution function */
SDL_ThreadFunction *Handler(void* ply){
    TCPsocket intermediary;
    char incoming[PACKETSIZE];
    tinfo* thread = (tinfo *) ply; // to be able to handle the threadinformation
    printf("Thread is active!\n");

    /* Sets the intermediary socket to handle the request */
    intermediary = SDLNet_TCP_Accept((*(*thread).socket));

    /* Recieves the request */
    SDLNet_TCP_Recv(intermediary, incoming, PACKETSIZE);

    switch(incoming[0]){
        case 'I':
            //send info
            return 0;
        case 'C':
            //check for serverslot
            return 0;
        default:
            //send error message, connection failed
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
    IPaddress listenerIP; // listen to the ip-address
    TCPsocket socket;
    
    /* Maximum number of sockets to be handeld*/
    SDLNet_SocketSet activity = SDLNet_AllocSocketSet(1); // functions that works with multiple sockets and allows you to determine when a socket has data or a connection waiting to be processed.
    
    HandlerInfo connectionhandler = {(*info).quit, &socket, (*info).stack};

    if(SDLNet_ResolveHost(&listenerIP,NULL,PORT) < 0){
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        return 1;
    }

    /* Open the sockets */
    if (!(socket = SDLNet_TCP_Open(&listenerIP))){
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        return 1;
    }

    SDLNet_AddSocket(activity, socket); // add socket to socket set

    while(!(*(*info).quit)){
        while(1){
            if(SDLNet_CheckSockets(activity, 50) > 0){ //Check and wait for sockets in a set to have activity
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
    (*ply).health = HEALTH;
    (*ply).position[0] = 5;
    (*ply).position[1] = 5;
}
