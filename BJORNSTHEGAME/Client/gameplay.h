#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED
#include <stdbool.h>
#include "bjornstartup.h"
#include "lobby.h"
#include "clientthreads.h"

#define SPEEDx 15
#define SPEEDy 10
#define GRAVITY 0.0000001

int gameplayWindow(ClientInfo* information);

int checkCollision( SDL_Rect a, SDL_Rect b );

int sendPlayerUpdate(playerInfo playerDummy, TCPsocket* socket);

int sendBulletUpdate(bullet bulletDummy, TCPsocket* socket);

#endif // GAMEPLAY_H_INCLUDED

