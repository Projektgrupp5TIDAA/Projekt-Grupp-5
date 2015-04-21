#include "bjornthreads.h"

/* Thread execution function */
SDL_ThreadFunction *check_ports(void* ply){
  char incoming[PACKETSIZE];
  tinfo thread = *((tinfo *) ply);
  printf("Thread #%d is active!\n", thread.threadID);
  while(1){
    if((*(thread.active)) == 1){
      if(SDLNet_TCP_Recv(*(thread.socket), incoming, PACKETSIZE)){
        if(strstr("EXITTHR", incoming)){
          printf("Thread #%d is now inactive!\n", thread.threadID);
          pushSocketStack(thread.servants, thread.threadID);
          *(thread.active) = 0;
        }else
          printf("Thread #%d says: %s\n", thread.threadID, incoming);
      }
    }
  }
}

/* Function for retrieving the location of Power-Up items */
void getPowerLocation(int loc[10][2]);

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply){
  (*ply).health = HEALTH;
  (*ply).drunkLevel = 0;
  (*ply).upgradeTimer = 0;
  (*ply).position[0] = 5;
  (*ply).position[1] = 5;
  //getPowerLocation((*ply).powerLocation);
  (*ply).damage = 1;
}
