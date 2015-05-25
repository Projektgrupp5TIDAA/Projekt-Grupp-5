/*
Includes all functions for threads having to do with the server
Created on 2015-05-12 by Jonathan Kåhre
*/
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "clientthreads.h"
#include "bjornstartup.h"
#include "lobby.h"
#include "gameplay.h"
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_net.h>
#include <SDL2/SDL_ttf.h>
#endif
#define PLAYERCOUNT 6
#define BULLETSPEED 30
#define GRAVITY 4


/* Function for handling all incoming information updates from the server */
int updateHandler(void* incinfo){
	updaterInfo* info = (updaterInfo*) incinfo;
	printf("Update poller running.\n");
	SDLNet_SocketSet activity = SDLNet_AllocSocketSet(1);
	SDLNet_AddSocket(activity, *(info->socket));
	char packet[512];
    int i, tmpID, activeplayers=0, errorcount=0, deathflag=0;

    /* The main polling-loop */
	while(!(*(info->quit))){
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
					parseString(packet, 1, sizeof(packet));
                    activeplayers = packet[0];
                    parseString(packet, 1, sizeof(packet));
                    printf("Recieved player update with %d active players!\n", activeplayers);
					memcpy(info->players, &packet, sizeof(playerInfo)*activeplayers);
					/*for(i=0;i<PLAYERCOUNT;i++)
						printf("Player %d: %d, %d\n", i, info->players[i]->pos.x, info->players[i]->pos.y);*/
					break;
				/*case 'C':
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
					break;*/
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
                case 'D':
                    parseString(packet, 1, sizeof(packet));
                    *(info->powerup) = atoi(packet);
                    break;
				default:
					printf("Invalid packet recieved, ignoring.\n");
                    if(errorcount > 20){
                        printf("Too many errors, exiting.\n");
                        SDL_Delay(2000);
                        (*(info->quit)) = 1;
                    }else errorcount++;
					break;
			}
		}else SDL_Delay(50);
	}
	printf("Updater thread exiting!\n");
	return 0;
}

int timeupdater(void* inc_time){
    timerInfo* timer = (timerInfo*) inc_time;
    int i, j, k, fall=1, deathflag=0, deathtime=0, timecheck=0;
    float acceleration = 0.5;
    Mix_Chunk* hurt = Mix_LoadWAV("../Sounds/hurt.wav");
    printf("Timer thread uppdater started\n");
    SDL_Delay(1000);
    while((*(timer->quit)) != 1){

        if(*(timer->timer) > 0){
            (*(timer->timer))--;
            timecheck ++;

            for(j=0;j<20;j++)
            {
                for(k=0;k<2;k++){
                	for(i=0; i<17; i++)
                    {
                        if(checkgravity(timer->animator->player->pos, timer->animator->platforms[i], GRAVITY*acceleration))
                        {
                           	fall = 0;
                            acceleration = 0.5;
                        }
                    }
                    if(fall == 1){
                        timer->animator->player->pos.y += GRAVITY*acceleration;
                        sendPlayerUpdate(*(timer->animator->player), timer->socket);
                        acceleration+=0.125;
                    }else
                        fall = 1;
                    SDL_Delay(25);
                }

	            for(i=0;i<12;i++){
                    if((timer->bullets[i]->TTL) > 0){
	                    timer->bullets[i]->pos.x += (BULLETSPEED*(timer->bullets[i]->direction));
	                    if(checkCollision(timer->bullets[i]->pos, timer->animator->player->pos)){
	                    	if(timer->animator->player->health > 0 && deathflag !=1){
                                    timer->animator->player->health -= timer->bullets[i]->dmg;
                                    Mix_HaltChannel(3);
                                    Mix_PlayChannel(3, hurt, 0);
                                    timer->bullets[i]->TTL = 0;
        
                                    if(timer->animator->player->health < 1){
                                        timer->animator->player->health = 0;
                                        timer->animator->player->deaths++;
                                        printf("This player died %d times\n", timer->animator->player->deaths);
                                        sendPlayerUpdate(*(timer->animator->player), timer->socket);
                                        deathflag = 1;
                                    }
	                    	}
                        }
                        else{
                            for(k=0;k<17;k++){
	                    	    if(checkCollision(timer->bullets[i]->pos, timer->animator->platforms[k]))
	                    	        timer->bullets[i]->TTL = 0;
	                    	    if(checkCollision(timer->bullets[i]->pos, timer->animator->players[k/2-1].pos) && timer->animator->players[k/2-1].health > 0)
	                    		    timer->bullets[i]->TTL = 0;
                            }
                        }
	                    if(timer->bullets[i]->TTL > 0)
	                    	timer->bullets[i]->TTL--;
	                }
                    
                    else if(deathtime > 3){
                        timer->animator->player->health = 5;
                        sendPlayerUpdate(*(timer->animator->player), timer->socket);
                        deathflag = 0;
                        deathtime = 0;
                    }
	                else if(timer->bullets[i]->pos.x != 0 && timer->bullets[i]->pos.y != 0){
	                    timer->bullets[i]->pos.x = 0;
	                    timer->bullets[i]->pos.y = 0;
	            	}
	        	}
	        }// for
            if(deathflag == 1){
                deathtime ++;
                printf("Deathtime is now :%d\n", deathtime);
            }
            printf("Gameplay time: %d is ticking\n", *(timer->timer));
        }else{
            if(timecheck > 1)
                (*(timer->quit)) = 1;
            /*else
                SDL_Delay(10);*/
        }
    }
    printf("Timeupdater exited!\n");
    return 0;
}

bool checkgravity(SDL_Rect a, SDL_Rect b, int modifier)
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h + modifier;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

/* Check if the bit at place 'bit' is set in int 'comp' */
int is_set(int comp, int bit){
    return (comp & 1<<bit);
}

/* Set the bit at place 'bit' of 'num' */
void set_bit(int *num, int bit){
    *num = *num | 1<<bit;
}

/* Clear the bit at place 'bit' of 'num' */
void clr_bit(int *num, int bit){
    if(is_set(*num, bit) > 0)
        *num = *num - (pow(2, bit));
}
