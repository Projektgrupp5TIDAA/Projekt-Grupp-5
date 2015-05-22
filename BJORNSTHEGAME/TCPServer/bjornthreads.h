/*
Functions required by the server to execute threads
Created 2015-04-16 by Jonathan K�hre
Edited by Dani Daryaweesh
Projekt Grupp 5
*/

#ifndef _BJORN_THREAD_
#define _BJORN_THREAD_
#ifdef __APPLE__
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_net.h>
#endif
#define PORT 4000
#define HEALTH 5
#define PLAYERCOUNT 6
#define PACKETSIZE 512
#define SERVERNAME "Server"
#define POWERTIMER 20
#define LOBBYLENGTH 2//120
#define GAMELENGTH 300

/* Struct with important information regarding the thread and player */
typedef struct{
  short health, deaths, dir;
  SDL_Rect pos;
}pinfo;

/* Struct with all the information a thread will have to get when getting a slot from the stack */
typedef struct{
    int ID, *powerup, *newdata;
    TCPsocket socket;
    pinfo* player;
    char playername[20];
    char* names[PLAYERCOUNT];
}tinfo;

/* Struct to send names */
typedef struct{
    char ID[PLAYERCOUNT];
    char names[PLAYERCOUNT][20];
}nsend;

/* The stack containing pointers to all of the information regarding the slots on the server */
typedef struct{
	int population;
	TCPsocket socket;
	tinfo* thread[PLAYERCOUNT];
}ThreadStack;

/* The struct for adding to stack from incoming data */
typedef struct{
    int population;
    char item[PLAYERCOUNT][PACKETSIZE];
}DataStack;

/* Struct with pointers required for the Poller to work */
typedef struct{
    int quit;
    ThreadStack stack;
    DataStack cstack;
    DataStack dstack;
}PollInfo;

/* Struct for counting down all the timers */
typedef struct{
    int maintimer, powerup;
}TimerInfo;

/* Struct with pointers required for the Clienthandlers to work */
typedef struct{
    int* quit;
    TCPsocket* socket;
    ThreadStack* stack;
    DataStack* cstack;
    DataStack* dstack;
}HandlerInfo;

#include "bjornstack.h"

/* Connection handler execution function */
int handler(void* ply);

/* Connection poller execution function */
int poller(void* information);

/* Thread counting down the main timer aswell as handle the powerup timers based on that */
int timer(void* information);

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply);

#endif
