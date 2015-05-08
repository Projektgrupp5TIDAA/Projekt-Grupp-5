#ifndef _LOBBY_H_

typedef struct{
    char ID[6];
    char names[6][20];
}nrecv;

int LobbyWindow(StartInfo lobbyConnection);

void parseString(char* inc, int hops, int len);

#endif // _LOBBY_H_

int readKeyboardToServer( StartInfo keyboard,SDL_Rect pos, char* output, int len, SDL_Window* window, SDL_Surface* bg);

/* time information in the lobby */ 
typedef struct{
    int *lobby_time;
}timeinfo;

SDL_Thread * timethr;