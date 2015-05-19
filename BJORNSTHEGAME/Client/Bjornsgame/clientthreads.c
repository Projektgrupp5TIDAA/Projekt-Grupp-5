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


/* Function for handling all incoming information updates from the server */
int updateHandler(void* incinfo){
	updaterInfo* info = (updaterInfo*) incinfo;
	printf("Update poller running.\n");
	SDLNet_SocketSet activity = SDLNet_AllocSocketSet(1);
	SDLNet_AddSocket(activity, *(info->socket));
	char packet[512], *message;
	SDL_Rect chat[20];
    char chatmessages[6][512]={{0}};
    TTF_Font* chatfont= TTF_OpenFont("../Images/menu/coolvetica.ttf", 18);
	int i, tmpID;

	SDL_Surface* screen = SDL_GetWindowSurface(info->window);

	/* Chat positions */
	for(i=0;i<6;i++){
        chat[i].x=(screen->w)/3 * 2 + 5;
        chat[i].y=(screen->h)/5 * 4 - (22*(i+1));
    }

    /* The main polling-loop */
	while(1){
		SDLNet_CheckSockets(activity, 30);

		/* Checks the socket for activity and if it finds it it will either
		1. Case 'P', update the positions of all players on the server.
		2. Case 'C', print the incoming chat message to the screen
		3. Case 'B', add a bullet to the field
		*/
		if(SDLNet_SocketReady(*(info->socket))){
			printf("Activity!\n");
			SDLNet_TCP_Recv(*(info->socket), &packet, sizeof(packet));
			switch(packet[0]){
				case 'P':
					printf("Playerupdate recieved!\n");
					parseString(packet, 1, sizeof(packet));
					memcpy(info->players, &packet, sizeof(playerInfo)*6);
					/*for(i=0;i<PLAYERCOUNT;i++)
						printf("Player %d: %d, %d\n", i, info->players[i]->pos.x, info->players[i]->pos.y);*/
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
                    printf("Bulletupdate recieved!\n");
                    parseString(packet, 1, sizeof(packet));
                    tmpID = packet[0];
                    parseString(packet, 1, sizeof(packet));
                    for(i=0;i<12;i++){
                    	if(info->bullets[i]->TTL == 0){
                    		memcpy(info->bullets[i], &packet, sizeof(*(info->bullets[i])));
                    		info->bullets[i]->ID = tmpID;
                    		printf("Bullet: %d, %d\n", info->bullets[i]->pos.x, info->bullets[i]->pos.y);
                    		break;
                    	}else{
                    		printf("Too many bullets!");
                    	}
                    }
					break;
                case 'T':
                    parseString(packet, 1, sizeof(packet));
                    info->timer=atoi(packet);
                    break;
				default:
					printf("Invalid packet recieved, ignoring.\n");
					break;
			}
		}else SDL_Delay(50);
		for(i=0;i<5;i++){
            textToScreen(chatfont, chat[i], info->window, chatmessages[i]);
        }
	}

	return 0;
}

int timeupdater(void* inc_time){
    timerInfo* timer= (timerInfo*) inc_time;
    int i, j;
    printf("Timer thread uppdater started\n");
    SDL_Delay(1000);
    while(!(*(timer->quit))){
        if(*(timer->timer) > 0){
            (*(timer->timer))--;

            for(j=0;j<20;j++){
	            for(i=0;i<12;i++){
	                if((timer->bullets[i]->TTL) > 0){
	                    timer->bullets[i]->pos.x += (BULLETSPEED*(timer->bullets[i]->direction));
	                    timer->bullets[i]->TTL--;
	                }else if(timer->bullets[i]->pos.x != 0 && timer->bullets[i]->pos.y != 0){
	                    timer->bullets[i]->pos.x = 0;
	                    timer->bullets[i]->pos.y = 0;
	            	}
	        	}
	        	SDL_Delay(50);
	        }

            printf("Gameplay time: %d is ticking\n", *(timer->timer));
        }else SDL_Delay(10);
    }
    return 0;
}