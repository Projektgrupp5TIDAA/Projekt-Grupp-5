#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "testheader.h"

int main(int argc, char const *argv[])
{
  SDL_Window* window;
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  window = SDL_CreateWindow("HEJ",				//Window name
			    SDL_WINDOWPOS_UNDEFINED,		//x-position
			    SDL_WINDOWPOS_UNDEFINED,		//y-position
			    600,
			    300,
			    0	//flags etc
			    );
  if (window==NULL)
    {
      printf("Window could not be created.\n");
    }
  
  SDL_Surface *screen = SDL_GetWindowSurface(window);
  SDL_Color white={255,255,255};
  TTF_Font *font = TTF_OpenFont("Metropolian-Text.ttf", 40);
  if(font == NULL){
    printf("Font can't be found or is in an unknown format.\n");
    exit(0);
  }
  SDL_Surface *text = TTF_RenderText_Solid(font, "I FIXED IT LOL", white);
  SDL_Surface *image = IMG_Load("yellow_player1.png");
  SDL_Rect imagedestination = {0,50,0,0};
  SDL_BlitSurface(image, NULL, screen, &imagedestination);
  SDL_BlitSurface(text, NULL, screen, NULL);
  SDL_UpdateWindowSurface(window);
  SDL_Delay(8000); //8 sek
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0;
  
}
