/*
Includes all functions for threads having to do with the server
Created on 2015-05-12 by Jonathan Kåhre
*/
#ifndef _CLIENTTHREADS_H_
#define _CLIENTTHREADS_H_
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif
#define PLAYERCOUNT 6
#define BULLETSPEED 30
#include "animation.h"
#include <stdbool.h>


/* Struct with the information needed by the client to render the players */
typedef struct{
    short health, deaths;
    SDL_Rect pos;
}playerInfo;

/* Struct to recieve bulletinformation */
typedef struct{
    SDL_Rect pos;
    short ID, TTL, direction, dmg;
}bullet;

/* Struct containing information on all the players */
typedef struct{
	int *quit, timer;
	TCPsocket* socket;
	playerInfo* players;
    bullet* bullets[12];
}updaterInfo;

typedef struct{
	int frame, *quit, *ammo, *drunk;
	SDL_Window* window;
	playerInfo *player;
	playerInfo players[6];
    SDL_RendererFlip flip;
    bullet bullets[12];
    SDL_Rect platforms[14];
    SDL_Rect bjornDrapare[2];
}animationInfo;

typedef struct{
	int* timer, *quit;
	bullet* bullets[12];
	animationInfo *animator;
	TCPsocket *socket;
}timerInfo;

int updateHandler(void* info);

int timeupdater(void* inc_time);

bool checkgravity( SDL_Rect a, SDL_Rect b );



#endif