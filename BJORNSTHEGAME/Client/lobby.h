#ifndef _LOBBY_H_

typedef struct{
    char ID[6];
    char names[6][20];
}nrecv;

int LobbyWindow(StartInfo lobbyConnection);

void parseString(char* inc, int hops, int len);

#endif // _LOBBY_H_

/* time information in the lobby */ 
typedef struct{
    int *lobby_time;
}timeinfo;

SDL_Thread * timethr;