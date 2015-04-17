#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "SDL2/SDL_net.h"
#define PORTS 4000
#include "bjornthreads.h"

int main(int argc, char **argv){
  IPaddress sourceIP, *destinationIP;
  TCPsocket socket, clientsockets[6];
  pinfo players[PLAYERCOUNT]; // Array of player-structs
  int ports[PLAYERCOUNT + 1]; // Array of ports
  getPorts(ports); //Populate the port-array
  char incomingdata[512], clear='0'; //char to clear the data input
  int stop=0, check_thread, threadID=0, i; //various ints
  pthread_t threads[PLAYERCOUNT + 1]; // Array av threads
  
  /* Initialize SDL_net */
  if (SDLNet_Init() < 0){
    fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }

  if(SDLNet_ResolveHost(&sourceIP,NULL,ports[0]) < 0){
      printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
  }
    
  /* Open the sockets */
  if (!(socket = SDLNet_TCP_Open(&sourceIP))){
    fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
    
  /* Main loop */
  while (!stop){
    /* Clears the data input then waits for a packet */
    if (clientsockets[threadID] = SDLNet_TCP_Accept(socket)){
      /* Copies the data and compares it to the request-string*/
      if(threadID < 6){
	/* If the request is valid, a thread is spawned and assigned a port */
	printf("Request recieved, assigning port.\n");
	initiatePlayer(&(players[threadID])); //Initiate the player-struct
	players[threadID].threadID = threadID;
	players[threadID].socket = clientsockets[threadID];
	pthread_create(&threads[threadID],NULL,&check_ports,(void *)&players[threadID]);
	threadID++;
      }else printf("Invalid request or server is full, discarding.\n");
    }		
  }
  int j;
  for(j=0;j<threadID;j++)
    pthread_join(threads[threadID], NULL);
  /* Clean and exit */
  SDLNet_TCP_Close(socket);
  SDLNet_Quit();
  return EXIT_SUCCESS;
}

