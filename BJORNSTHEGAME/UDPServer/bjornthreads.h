#ifndef _BJORN_THREAD_
#define _BJORN_THREAD_
#define HEALTH 5
#define PLAYERCOUNT 6

/* Struct with important information regarding the thread and player */
typedef struct playerinfo{
  int health, drunkLevel, upgradeTimer, position[2], powerLocation[10][2], damage, threadID, port;
}pinfo;

/* Thread execution function */
void *check_ports(void* ply){
  UDPsocket socket;
  UDPpacket *packet;
  pinfo player = *((pinfo *) ply);
  if(!(socket = SDLNet_UDP_Open(player.port))){
    fprintf(stderr, "SDLNet_UDP_Open for thread %d: %s", player.threadID, SDLNet_GetError());
  }
  if(!(packet = SDLNet_AllocPacket(512))){
    fprintf(stderr, "SDLNet_Allocation %s for thread %d", SDLNet_GetError(), player.threadID);
  }
  SDLNet_UDP_Recv(socket, packet);
  printf("Thread with ID: %d is working!\n", player.threadID);
  while(1){
    if(SDLNet_UDP_Recv(socket, packet)){
      printf("Thread #%d says: %s\n", player.threadID, packet->data);
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

/* Gets the port-range to be used by the server */
void getPorts(int ports[PLAYERCOUNT + 1]){
  int i;
  for(i=0;i<PLAYERCOUNT+1;i++){ 
    ports[i] = PORTS + i;
  }
}

#endif
