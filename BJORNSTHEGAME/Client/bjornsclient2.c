#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "testheader.h"

int main(int argc, char const *argv[])
{
  SDL_Window* window;
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  window = SDL_CreateWindow("HEJ",						//Window name
			    SDL_WINDOWPOS_UNDEFINED,		//x-position
			    SDL_WINDOWPOS_UNDEFINED,		//y-position
			    200,
			    200,
			    0	//flags etc
			    );
  if (window==NULL)
    {
      printf("Couldnt create window\n");
    }

  SDL_Surface *screen = SDL_GetWindowSurface(window);
  printf("Hej1\n");
  SDL_Color black={0,0,0};
  printf("Hej2\n");
  TTF_Font *font = TTF_OpenFont("Sans.ttf", 16);
  if(font == NULL)
    printf("FONT E NULL\n");
  printf("Hej3\n");
  SDL_Surface *test = TTF_RenderText_Solid(font, "HEJ", black);
  printf("Hej4\n");
  SDL_BlitSurface(test, NULL, screen, NULL); 
  printf("Hej5\n");
  SDL_Delay(8000); //8 sek
  printf("Hej6\n");
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0;
  
}
