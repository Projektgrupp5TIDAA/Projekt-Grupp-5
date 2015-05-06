typedef struct clockInformation
{
    int seconds_left;
    char sendingTime[3];

}clkInfo;

typedef struct{
    char ID[6];
    char names[6][20];
}nrecv;

int LobbyWindow(StartInfo lobbyConnection);
int TimeThread(SDL_Window* screen); 

int Players_names(StartInfo players , SDL_Window* lobbyscreen, SDL_Surface* lobbySurface); 
