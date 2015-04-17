#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "SDL2/SDL_net.h"
#define PORTS 2000
#include "bjornthreads.h"

int main(int argc, char **argv){
  UDPsocket sock[PLAYERCOUNT + 1];       /* Socket descriptor */
  UDPpacket *mem;       /* Pointer to packet memory */
  pinfo players[PLAYERCOUNT];
  int ports[PLAYERCOUNT + 1];
  getPorts(ports);
  char clear='0';
  int open[PLAYERCOUNT + 1] = {0};
  int quit[PLAYERCOUNT + 1] = {0};
  int check_thread, threadID=0, i;
  pthread_t threads[PLAYERCOUNT + 1]; // array av trådar
  
  /* Initialize SDL_net */
  if (SDLNet_Init() < 0){
    fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
  
  /* Open the sockets */
  for(i=0;i<PLAYERCOUNT+1;i++){
    if (!(sock[i] = SDLNet_UDP_Open(ports[i]))){
      fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
      exit(EXIT_FAILURE);
    }
    open[0]=1;
  }
  /* Make space for the packets on the sockets */
  if (!(mem = SDLNet_AllocPacket(512))){
    fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
    exit(EXIT_FAILURE);
  }
  
  /* Main loop */
  while (!quit[0]){
    /* Waits for a packet. UDP_Recv returns != 0 if a packet is incoming */
    mem->data = &clear;
    if (SDLNet_UDP_Recv(sock[0], mem)){
      /* Copies the data and compares it to the request-string*/
      if(strstr("COMMREQ", mem->data) && threadID < 7){
	/* If the request is valid, a thread is spawned and assigned a socket with a port-number */
	printf("Request recieved, assigning port.\n");
	initiatePlayer(&(players[threadID]));
	players[threadID].threadID = threadID;
	players[threadID].sock = &sock[threadID+1];//+1 because 0 is main
	open[threadID] = 1;
	char sendport[4];
	sprintf(sendport, "%d", ports[threadID+1]);
	mem->data = sendport;
	SDLNet_UDP_Send(sock[threadID+1], -1, mem);
	pthread_create(&threads[threadID],NULL,&check_ports,(void *)&players[threadID]);
	threadID++;
      }else printf("Invalid request recieved, discarding.\n");
      
      /* Quit if packet contains the exit-command */
      if (strcmp((char *)mem->data, "EXIT") == 0)
	quit[0] = 1;
    }		
  }
  int j;
  for(j=0;j<threadID;j++)
    pthread_join(threads[threadID], NULL);
  /* Clean and exit */
  SDLNet_FreePacket(mem);
  SDLNet_Quit();
  return EXIT_SUCCESS;
}

