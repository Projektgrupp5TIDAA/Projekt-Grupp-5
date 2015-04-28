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
#define SERVERNAME Server

/* Struct with important information regarding the thread and player */
typedef struct playerinfo{
  int health, position[2];
  char playername[20];
}pinfo;

typedef struct ThreadInformation{
    int ID;
    TCPsocket* socket;
    pinfo* player;
}tinfo;

/* struct with important information about the sockets id */
typedef struct Threadstack{
	int population;
	TCPsocket* socket;
	tinfo* thread[PLAYERCOUNT]; // array of threadinformation
}ThreadStack;

/* struct with info regarding the stack */
typedef struct PollerInformation{
    int* quit;
    ThreadStack* stack;
}PollInfo;

/* struct with info regarding the required connection variables*/
typedef struct HandlerInformation{
    int* quit;
    TCPsocket* socket;
    ThreadStack* stack;
}HandlerInfo;

#include "bjornstack.h"

/* Connection handler execution function */
SDL_ThreadFunction* handler(void* ply);

/* Connection poller execution function */
SDL_ThreadFunction* poller(void* information);

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply);

#endif
