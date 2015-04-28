#include "bjornstartup.h"
#include "lobby.h"
#define PACKETSIZE 512
int main(int argc, char *argv[])
{
  TCPsocket socket;
  IPaddress targethost;
  char TCPData[PACKETSIZE];
  SDL_Window* window;
  SDL_Color white={255,255,255};
  StartInfo startup = {&socket, &targethost, {0}};// array of structs
  startup.socket = &socket; // access to socket and targethost 
  startup.targethost = &targethost;

  /* Initialization, add error checks */
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  SDLNet_Init();
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

  /* Create window and get the surface */
  window = SDL_CreateWindow("BJORNS THE GAME",				//Window name
			    SDL_WINDOWPOS_UNDEFINED,		//x-position
			    SDL_WINDOWPOS_UNDEFINED,		//y-position
			    800,
			    600,
			    0	//flags etc
			    );
  if (window==NULL)
    {
      printf("Window could not be created.\n");
    }
  
  SDL_Surface *screen = SDL_GetWindowSurface(window);

  /* Call the menu */
  menu(window, startup);
  
  /* Clean up */
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0;
  
}
