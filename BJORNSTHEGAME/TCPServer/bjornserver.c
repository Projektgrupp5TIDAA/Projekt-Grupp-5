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
  int currentsocket=0, quit=0, i, activethread[PLAYERCOUNT] = {0}; //various ints
  SDL_Thread* threads[PLAYERCOUNT]; // Array of threads
  
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
    threadvariables[i].quit = &quit;
    threads[i] = SDL_CreateThread(check_ports, "Thread", (void *)&threadvariables[i]); //Creates the thread
    SDL_DetachThread(threads[i]); //  let a thread clean up on exit without intervention
  }

  /* Main loop */
  /* Maybe use another thread for connectionpolling and keep the main thread for broadcasts? */
  while(!quit){
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
    }else SDL_Delay(200);
  }

  SDLNet_TCP_Close(socket);
  SDLNet_Quit();
  return EXIT_SUCCESS;
}

