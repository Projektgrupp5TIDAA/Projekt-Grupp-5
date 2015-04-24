#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bjornthreads.h"
#include "bjornstack.h"
#include "bjornshared.h"

int main(int argc, char **argv){
  IPaddress sourceIP;
  TCPsocket socket, clientsockets[PLAYERCOUNT];
  cServ servants = {0,{0}}; //Stack
  tinfo threadvariables[PLAYERCOUNT]; // Array of player-structs
  char incomingdata[512]; //char to clear the data input
  int currentsocket=0, stop=0, i, activethread[PLAYERCOUNT] = {0}; //various ints
  SDL_Thread* threads[PLAYERCOUNT + 1]; // Array of threads
  
  /* Initialize SDL_net */
  if (SDLNet_Init() < 0){
    fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }

  if(SDLNet_ResolveHost(&sourceIP,NULL,PORT) < 0){
      printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
  }
    
  /* Open the sockets */
  if (!(socket = SDLNet_TCP_Open(&sourceIP))){
    fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
    

  //Put in a function(Populate stack)
  for(i=0;i<PLAYERCOUNT;i++){
    pushSocketStack(&servants, i); //Pushes the current socket that is being handled onto the stack
    initiatePlayer(&(threadvariables[i].player));  //Initiate the player-struct
    threadvariables[i].active = &activethread[i];  //Gives the current thread the active flag-pointer
    threadvariables[i].servants = &servants;       //Gives the thread access to the stack
    threadvariables[i].threadID = i;               //Assigns the thread ID
    threadvariables[i].socket = &clientsockets[i]; //Assigns the thread it's socket
    SDL_CreateThread(check_ports, "Thr", (void *)&threadvariables[i]); //Creates the thread
  }

  /* Main loop */
  while(!stop){
    /* Clears the data input then waits for a packet */
    if(!isEmptyStack(servants)){
      currentsocket = popSocketStack(&servants); // take socket id from the stack 
      while(1){
        if((clientsockets[currentsocket] = SDLNet_TCP_Accept(socket))){
         activethread[currentsocket] = 1;
         printf("Request recieved, assigning socket to thread #%d.\n", currentsocket);
         break;
        }
      }
    }   
    //BROADCAST
      /*for(i=0;i<PLAYERCOUNT;i++){
          
      }*/ 
  }

  SDLNet_TCP_Close(socket);
  SDLNet_Quit();
  return EXIT_SUCCESS;
}

