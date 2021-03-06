#include "bjornstartup.h"
#include "lobby.h"
#include "gameplay.h"
#define PACKETSIZE 512

int main(int argc, char *argv[]){
    ClientInfo startup = {NULL, {0, 0}, {0}, 0};

    if(argc > 1){
        if(strstr(argv[1], "local")){
            startup.directConnect = 1;
        }else if(strstr(argv[1], "remote")){
            startup.directConnect = 1;
        }else{
            printf("No valid arguments.\n");
            return 0;
        }
    }

    /* Initialization, add error checks */
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDLNet_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    /* Call the menu */
    if(menu(&startup)){
    printf("Menu quit or exited with an error.\n");
        return 0;
    }
    Mix_HaltMusic();

    /* if exit is pressed, exit the game */
    if(LobbyWindow(&startup)==1){
        SDLNet_TCP_Send(startup.socket, "EXITCONNECTION", 16);
        printf("Exiting the game!\n");
        return 0;
    }

    /* Clean up */
    printf("Going to the gameplay!\n");
    gameplayWindow(&startup);

    if((startup.socket) != NULL)
        SDLNet_TCP_Send(startup.socket, "EXITCONNECTION", 16);
    SDL_Quit();
    TTF_Quit();
    SDLNet_Quit();
    return 0;
}
