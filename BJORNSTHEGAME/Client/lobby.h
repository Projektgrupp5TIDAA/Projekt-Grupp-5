typedef struct{
    char ID[6];
    char names[6][20];
}nrecv;

int LobbyWindow(StartInfo lobbyConnection);

int Players_names(StartInfo players , SDL_Window* lobbyscreen, SDL_Surface* lobbySurface); 
