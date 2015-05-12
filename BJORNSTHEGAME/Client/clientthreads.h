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

/* Struct with the information needed by the client to render the players */
typedef struct{
    short health, kills;
    SDL_Rect pos;
}playerInfo;

/* Struct to recieve bulletinformation */
typedef struct{

}brecv;

/* Struct containing information on all the players */
typedef struct{
	SDL_Window* window;
	TCPsocket* socket;
	playerInfo players[PLAYERCOUNT];
}updaterInfo;

int updateHandler(void* info);

int timeupdater(void* inc_time);

#endif