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
#define SERVERNAME "Server"
#define POWERTIMER 3
#define LOBBYLENGTH 15
#define GAMELENGTH 30

/* Struct with important information regarding the thread and player */
typedef struct playerinfo{
  short health, position[2];
  char playername[20];
}pinfo;

/* Struct with all the information a thread will have to get when getting a slot from the stack */
typedef struct ThreadInformation{
    int ID, *active;
    TCPsocket* socket;
    pinfo* player;
    char* names[PLAYERCOUNT];
}tinfo;

/* The stack containing pointers to all of the information regarding the slots on the server */
typedef struct Threadstack{
	int population;
	TCPsocket* socket;
	tinfo* thread[PLAYERCOUNT];
}ThreadStack;

/* The struct for adding to stack from incoming data */
typedef struct Datastack{
    int population;
    char item[PLAYERCOUNT];
}DataStack;

/* Struct with pointers required for the Poller to work */
typedef struct PollerInformation{
    int* quit;
    ThreadStack* stack;
}PollInfo;

/* Struct for counting down all the timers */
typedef struct TimerInformation{
    int *main, *powerup;
}TimerInfo;

/* Struct with pointers required for the Clienthandlers to work */
typedef struct HandlerInformation{
    int* quit;
    TCPsocket* socket;
    ThreadStack* stack;
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
