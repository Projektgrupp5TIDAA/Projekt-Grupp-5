/*
Includes all functions for threads having to do with the server
Created on 2015-05-12 by Jonathan Kåhre
*/

#include <stdio.h>
#include <string.h>
#include "clientthreads.h"
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif

int updateHandler(void* incinfo){
	updaterInfo* info = (updaterInfo*) incinfo;
	printf("Update poller running.\n");
	SDLNet_SocketSet activity = SDLNet_AllocSocketSet(1);
	SDLNet_AddSocket(activity, *(info->socket));
	char packet[512];

	while(1){
		SDLNet_CheckSockets(activity, 30);
		if(SDLNet_SocketReady(*(info->socket))){
			SDLNet_TCP_Recv(*(info->socket), &packet, sizeof(packet));
			switch(packet[0]){
				case 'P':
					break;
				case 'C':
					break;
				case '':
					break;
				default:
					printf("Invalid packet recieved, ignoring.\n");
					break;
			}
		}
	}

	return 0;
}