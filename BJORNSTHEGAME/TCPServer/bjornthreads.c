#include "bjornthreads.h"

/* Thread execution function */
SDL_ThreadFunction *check_ports(void* ply){
  char incoming[PACKETSIZE];
  int name = 0;
  tinfo* thread = (tinfo *) ply;
  printf("Thread #%d is active!\n", (*thread).threadID);

  /* Main loop of the thread, only run if the thread is set to active */
  while(1){
    if((*(*thread).active) == 1){
      /* Exit the thread if the program is stopped */
      if((*(*thread).quit) == 1){
          SDLNet_TCP_Send((*(*thread).socket), "SERVERSHUTDOWN", 14);
          return 0;
      }

      /* Set the name at the start of connection */
      if(name == 0){
          if(SDLNet_TCP_Recv(*((*thread).socket), (*thread).player.playername, 20))
          printf("Player named: %s connected!\n", (*thread).player.playername);
          name = 1;
          SDLNet_TCP_Recv(*((*thread).socket), incoming, PACKETSIZE);
      }else{

        /* If a packet is recieved, check if for the flags or if it's a end of connection transmission */
        if(SDLNet_TCP_Recv(*((*thread).socket), incoming, PACKETSIZE)){
          if(strstr("EXITCONNECTION", incoming)){
            printf("Thread #%d is now inactive!\n", (*thread).threadID);
            pushSocketStack((*thread).servants, (*thread).threadID); // pushes sockets id to the stack 
            *((*thread).active) = 0;
            name = 0;
          }else

          /* Parse and post if the flag is set to chat */
          if(incoming[0] == 'C'){
            parseChat(incoming, 1, strlen(incoming));
            if(strstr("tapir", incoming)){
              (*thread).player.drunkLevel = 4;
              printf("Codeword detected! Drunklevel of %s set to Phöz.\n", (*thread).player.playername);
            }else{
              printf("%s says: %s\n", (*thread).player.playername, incoming);
            }
          }
          
          /* Functions for data will be placed here */
          if(incoming[0] == 'D'){
            printf("Data recieved.\n");
          }
        }
      }
    }else SDL_Delay(200); //If this isn't here the server will constantly take all available CPU-power unless otherwise limited.
  }
}

/* Initiation of the player struct */
void initiatePlayer(pinfo* ply){
  (*ply).health = HEALTH;
  (*ply).drunkLevel = 0;
  // (*ply).upgradeTimer = 0;
  (*ply).position[0] = 5;
  (*ply).position[1] = 5;
  //getPowerLocation((*ply).powerLocation);
  (*ply).damage = 1;
}
