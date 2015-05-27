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
#include "animation.h"
#include "bjornstartup.h"
#include <stdbool.h>
#define PLAYERCOUNT 6
#define BULLETSPEED 30
#define GRAVITY 4
#define PACKETSIZE 512


/* Struct with the information needed by the client to render the players */
typedef struct{
    short health, deaths, dir;
    SDL_Rect pos;
}playerInfo;

/* Struct to recieve bulletinformation */
typedef struct{
    SDL_Rect pos;
    short ID, TTL, direction, dmg;
}bullet;

/* Struct containing information on all the players */
typedef struct{
	int *quit, *powerup, timer;
	TCPsocket* socket;
	playerInfo* players;
    bullet* bullets[12];
}updaterInfo;

/* struct containing all animation information */
typedef struct{
	int frame, *quit, *ammo, *drunk, *powerup, *gameclock;
	SDL_Window* window;
	playerInfo *player;
	playerInfo players[6];
    SDL_RendererFlip flip;
    bullet bullets[12];
    SDL_Rect platforms[17];
    SDL_Rect bjornDrapRect[3];
    nrecv* names;
}animationInfo;

/* struct containing all time information */
typedef struct{
	int* timer, *quit;
	bullet* bullets[12];
	animationInfo *animator;
	TCPsocket *socket;
}timerInfo;

int updateHandler(void* info);

int timeupdater(void* inc_time);

bool checkgravity(SDL_Rect a, SDL_Rect b, int modifier);

int is_set(int comp, int bit);

void set_bit(int *num, int bit);

void clr_bit(int *num, int bit);

int unpackPlayerPacket(char packet[PACKETSIZE], playerInfo players[PLAYERCOUNT], int activeplayers);

int unpackBulletPacket(char* packet, bullet* bullet);

#endif
