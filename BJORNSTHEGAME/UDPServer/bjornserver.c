#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "SDL2/SDL_net.h"
#define PORTS 2000
#include "bjornthreads.h"

int main(int argc, char **argv){
  UDPsocket socket;       // Socket
  UDPpacket *packet;       // Pointer to the allocated space of the socket
  pinfo players[PLAYERCOUNT]; // Array of player-structs
  int ports[PLAYERCOUNT + 1]; // Array of ports
  getPorts(ports); //Populate the port-array
  char clear='0'; //char to clear the data input
  int quit=0, check_thread, threadID=0, i; //various ints
  pthread_t threads[PLAYERCOUNT + 1]; // Array av threads
  
  /* Initialize SDL_net */
  if (SDLNet_Init() < 0){
    fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
  
  /* Open the sockets */
  if (!(socket = SDLNet_UDP_Open(ports[0]))){
    fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
  
  /* Allocate space for the sockets */
  if (!(packet = SDLNet_AllocPacket(512))){
    fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
  
  /* Main loop */
  while (!quit){
    /* Clears the data input then waits for a packet */
    packet->data = &clear;
    if (SDLNet_UDP_Recv(socket, packet)){
      /* Copies the data and compares it to the request-string*/
      if(strstr("COMMREQ", packet->data) && threadID < 6){
	/* If the request is valid, a thread is spawned and assigned a port */
	printf("Request recieved, assigning port.\n");
	initiatePlayer(&(players[threadID])); //Initiate the player-struct
	players[threadID].threadID = threadID;
	players[threadID].port = ports[threadID+1];//+1 because 0 is main
	char sendport[4];
	sprintf(sendport, "%d", ports[threadID+1]);
	packet->data = sendport;
	SDLNet_UDP_Send(socket, -1, packet);
	pthread_create(&threads[threadID],NULL,&check_ports,(void *)&players[threadID]);
	threadID++;
      }else printf("Invalid request or server is full, discarding.\n");
      
      /* Quit if packet contains the exit-command */
      if (strcmp((char *)packet->data, "EXIT") == 0)
	quit = 1;
    }		
  }
  int j;
  for(j=0;j<threadID;j++)
    pthread_join(threads[threadID], NULL);
  /* Clean and exit */
  SDLNet_FreePacket(packet);
  SDLNet_Quit();
  return EXIT_SUCCESS;
}

