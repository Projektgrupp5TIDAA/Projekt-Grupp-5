#include "bjornstartup.h"

int main(int argc, char *argv[])
{
  TCPsocket socket;
  IPaddress targethost;
  char TCPData[512];
  SDL_Window* window;
  SDL_Color white={255,255,255};
  StartInf startup = {&socket, &targethost, {0}};// array of structs
  startup.socket = &socket; // access to socket and targethost 
  startup.targethost = &targethost;

  /*TTF_Font *font = TTF_OpenFont("../Images/menu/StencilStd.ttf", 40);
  if(font == NULL){
    printf("Font can't be found or is in an unknown format.\n");
    exit(0);
  }
  SDL_Surface *text = TTF_RenderText_Solid(font, "I FIXED IT LOL", white);
  SDL_Surface *image = IMG_Load("yellow_player1.png");
  SDL_Surface *bg = IMG_Load("../Images/Backgrounds/Action-movie-Background1.jpg");
  SDL_Rect imagedestination = {0,50,45,75};*/

  /* Initialization, add error checks */
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  SDLNet_Init();
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

  /* Create window and get the surface */
  window = SDL_CreateWindow("HEJ",				//Window name
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

  /* Draw everything then wait */
  //while(!quit){
  //getGameState(window, TCPData);  
  //SDL_UpdateWindowSurface(window);
  //}
  /*
  SDL_BlitSurface(bg, NULL, screen, NULL);
  SDL_BlitScaled(image, NULL, screen, &imagedestination);
  SDL_BlitSurface(text, NULL, screen, NULL);
  SDL_UpdateWindowSurface(window);
  SDL_Delay(1000); //8 sek*/
  
  /* Clean up */
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0;
  
}
