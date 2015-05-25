#ifndef _LOBBY_H_
#define _LOBBY_H_

typedef struct{
	int *timer, *quit;
}lbytmr;

int LobbyWindow(ClientInfo* lobbyConnection);

void parseString(char* inc, int hops, int len);

int timepoll(void* inctimer);

void convertTimer(char output[5], int input);

#endif // _LOBBY_H_