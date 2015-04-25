#ifndef _BJORN_THREAD_
#define _BJORN_THREAD_
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif
#include <SDL2/SDL_thread.h>
#define PORT 4000
#define HEALTH 5
#define PLAYERCOUNT 6
#define PACKETSIZE 512

/* struct with important information about the sockets id */
typedef struct clientServant{
	int population, socket[PLAYERCOUNT];
}cServ;

#include "bjornstack.h"

/* Struct with important information regarding the thread and player */
typedef struct playerinfo{
  int health, drunkLevel, position[2], powerLocation, damage;
  char playername[20];
}pinfo;

/* struct with information about the threads */
typedef struct threadinfo{
	TCPsocket *socket;
	int threadID, *active, *quit;
	pinfo player;
	cServ *servants;
}tinfo;

/* Thread execution function */
SDL_ThreadFunction *check_ports(void* ply);

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply);

#endif
