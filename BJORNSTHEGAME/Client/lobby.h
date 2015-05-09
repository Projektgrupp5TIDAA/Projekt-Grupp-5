#ifndef _LOBBY_H_

typedef struct{
    char ID[6];
    char names[6][20];
}nrecv;

int LobbyWindow(StartInfo* lobbyConnection);

void parseString(char* inc, int hops, int len);

int timepoll(void* inctimer);

void convertTimer(char output[5], int input);

#endif // _LOBBY_H_