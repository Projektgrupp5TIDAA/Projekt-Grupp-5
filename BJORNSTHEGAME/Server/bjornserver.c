#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "SDL2_net/SDL_net.h"

void *check_ports(){
    printf("Tråden funkar!");
}

 
int main(int argc, char **argv)
{
	UDPsocket sd[7];       /* Socket descriptor */
	UDPpacket *p[7];       /* Pointer to packet memory */
	int open[7] = {0};
	int quit[7] = {0};
    int check_thread, i=1;
    pthread_t threads[6]; // array av trådar
 
	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Open a socket */
	if (!(sd[0] = SDLNet_UDP_Open(2000)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	open[0]=1;
	
	/* Make space for the packet */
	if (!(p[0] = SDLNet_AllocPacket(512)))
	{
		fprintf(stderr, "SDLNet_AllocPacket: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
 
	/* Main loop */
	while (!quit[0])
	{
		/* Wait a packet. UDP_Recv returns != 0 if a packet is coming */
		if (SDLNet_UDP_Recv(sd[0], p[0]))
		{
		  //printf("UDP Packet incoming\n");
			printf("\tChan:    %d\n", p[0]->channel);
			//printf("\tData:    %s\n", (char *)p[0]->data);
			char req[100];
			strcpy(req, p[0]->data);
			if(strstr("COMMREQ", req)){
                printf("\n\nRequest recieved, assigning port.\n");
                pthread_create(&threads[i],NULL,&check_ports,NULL);
			}else printf("\n\nInvalid request recieved, discarding.\n");
			printf("\n\n\tLen:     %d\n", p[0]->len);
			printf("\tMaxlen:  %d\n", p[0]->maxlen);
			printf("\tStatus:  %d\n", p[0]->status);
			printf("\tAddress: %x %x\n", p[0]->address.host, p[0]->address.port);
 
			/* Quit if packet contains "quit" */
			if (strcmp((char *)p[0]->data, "quit") == 0)
				quit[0] = 1;
            i++;
		}		
	}
    int j;
    for(j=0;j<i;j++)
        pthread_join(threads[i], NULL);
	/* Clean and exit */
	SDLNet_FreePacket(p[0]);
	SDLNet_Quit();
	return EXIT_SUCCESS;
}

