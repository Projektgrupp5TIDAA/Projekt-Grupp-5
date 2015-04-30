#include "bjornstartup.h"
#include "lobby.h"
#define PACKETSIZE 512

int main(int argc, char *argv[]){
    IPaddress targethost;
    TCPsocket socket;
    StartInfo startup = {&socket, &targethost, {0}};
    startup.socket = &socket;
    startup.targethost = &targethost;

    /* Initialization, add error checks */
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDLNet_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

    /* Call the menu */
    if(menu(startup)){
        printf("Menu quit or exited with an error.\n");
        return 0;
    }
    Mix_HaltMusic();
    LobbyWindow(startup);

    /* Clean up */
    SDLNet_TCP_Send(socket, "EXITCONNECTION", 16);
    SDL_Quit();
    TTF_Quit();
    SDLNet_Quit();
    return 0;
}
