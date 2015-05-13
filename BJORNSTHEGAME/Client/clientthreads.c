/*
Includes all functions for threads having to do with the server
Created on 2015-05-12 by Jonathan Kåhre
*/

#include <stdio.h>
#include <string.h>
#include "clientthreads.h"
#include "bjornstartup.h"
#include "lobby.h"
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#endif

int updateHandler(void* incinfo){
	updaterInfo* info = (updaterInfo*) incinfo;
	printf("Update poller running.\n");
	SDLNet_SocketSet activity = SDLNet_AllocSocketSet(1);
	SDLNet_AddSocket(activity, *(info->socket));
	char packet[512], *message;
	SDL_Rect chat[20];
    char chatmessages[6][512]={{0}};
    TTF_Font* chatfont= TTF_OpenFont("../Images/menu/coolvetica.ttf", 18);
	int i;

	SDL_Surface* screen = SDL_GetWindowSurface(info->window);

	for(i=0;i<6;i++){
        chat[i].x=(screen->w)/3 * 2 + 5;
        chat[i].y=(screen->h)/5 * 4 - (22*(i+1));
    }

	while(1){
		SDLNet_CheckSockets(activity, 30);
		if(SDLNet_SocketReady(*(info->socket))){
			printf("Activity!\n");
			SDLNet_TCP_Recv(*(info->socket), &packet, sizeof(packet));
			switch(packet[0]){
				case 'P':
					printf("Playerupdate recieved!\n");
					parseString(packet, 1, sizeof(packet));
					memcpy(&info->players, &packet, sizeof(playerInfo));
					for(i=0;i<PLAYERCOUNT;i++)
						printf("Player %d: %d, %d\n", i, info->players[i].pos.x, info->players[i].pos.y);
					break;
				case 'C':
					printf("Chat recieved!\n");
					parseString(packet,1,strlen(packet));
                    for(i=5;i>0;i--){
                        strcpy(chatmessages[i], chatmessages[i-2]);
                    }
                    message = strtok(packet, "\n");
                    message = strtok(NULL, "\n");
                    strcpy(chatmessages[0], message);
                    emptyString(message, strlen(message));
                    strcpy(chatmessages[1], packet);
					break;
				case 'B':
					//createBullet();
					break;
				default:
					printf("Invalid packet recieved, ignoring.\n");
					break;
			}
		}
		for(i=0;i<5;i++){
            textToScreen(chatfont, chat[i], info->window, chatmessages[i]);
        }
	}

	return 0;
}

int timeupdater(void * inc_time){
    int* tmp_timer= (int*) inc_time;
    printf("Timer thread uppdater started\n");
    SDL_Delay(1000);
    while(1){
        if((*(tmp_timer)) > 0){
            (*(tmp_timer))--;
            printf("Gameplay time: %d is ticking\n", *tmp_timer);
        }
        SDL_Delay(995);
    }
    return 0;
}