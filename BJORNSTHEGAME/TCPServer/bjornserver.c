#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_net.h>
#include "bjornthreads.h"

int main(int argc, char **argv){
  IPaddress sourceIP, *destinationIP;
  TCPsocket socket, clientsockets[6];
  tinfo threadvariables[PLAYERCOUNT]; // Array of player-structs
  char incomingdata[512]; //char to clear the data input
  int stop=0, threadID=0, i; //various ints
  SDL_Thread* threads[PLAYERCOUNT + 1]; // Array of threads
  int activethreads[PLAYERCOUNT] = {0};
  
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
    
  /* Main loop */
  while(!stop){
    /* Clears the data input then waits for a packet */
    if (clientsockets[threadID] = SDLNet_TCP_Accept(socket)){
      /* Copies the data and compares it to the request-string*/
      if(threadID < 6){
	     /* If the request is valid, a thread is spawned and assigned a port */
	     printf("Request recieved, assigning socket to thread #%d.\n", threadID);
	     initiatePlayer(&(threadvariables[threadID].player)); //Initiate the player-struct
	     threadvariables[threadID].threadID = threadID;
	     threadvariables[threadID].socket = clientsockets[threadID];
       activethreads[threadID] = 1;
       threadvariables[threadID].active = &activethreads[threadID];
	     SDL_CreateThread(check_ports, "Thr", (void *)&threadvariables[threadID]);
       if(threadID == 5)
       stop = 1;
       threadID++;
      } 
    }		
  }
  stop = 0;
  while(!stop){
    //if(SDLNet_SocketReady(socket)){
      for(i=0;i<PLAYERCOUNT;i++){
        if(activethreads[i] == 0){
          printf("Socket #%d closed, ready for new connection!\n", i);
          SDLNet_TCP_Close(clientsockets[i]);
        }
      }
  }
  int j;
  for(j=0;j<threadID;j++)
    SDL_WaitThread(threads[threadID], NULL);
  /* Clean and exit */
  SDLNet_TCP_Close(socket);
  SDLNet_Quit();
  return EXIT_SUCCESS;
}

