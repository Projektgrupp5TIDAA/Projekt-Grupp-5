#ifndef _BJORN_THREAD_
#define _BJORN_THREAD_
#define PORT 4000
#define HEALTH 5
#define PLAYERCOUNT 6
#define PACKETSIZE 512

/* Struct with important information regarding the thread and player */
typedef struct playerinfo{
  int health, drunkLevel, upgradeTimer, position[2], powerLocation[10][2], damage;
}pinfo;

typedef struct threadinfo{
	TCPsocket socket;
	int threadID, *active;
	pinfo player;
}tinfo;

/* Thread execution function */
SDL_ThreadFunction *check_ports(void* ply);

/* Function for retrieving the location of Power-Up items */
void getPowerLocation(int loc[10][2]);

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply);

/* Gets the port-range to be used by the server */
void getPorts(int ports[PLAYERCOUNT + 1]);

#endif
