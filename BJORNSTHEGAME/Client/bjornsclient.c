#include <stdio.h>
#include <sdl2/sdl.h>
#include <sdl2_net/sdl_net.h>
#include "testheader.h"

int main(int argc, char const *argv[])
{
	SDL_Window* window;
	SDL_INIT_VIDEO;
    window = SDL_CreateWindow("HEJ",						//Window name
    						SDL_WINDOWPOS_UNDEFINED,		//x-position
    						SDL_WINDOWPOS_UNDEFINED,		//y-position
    						720,
    						400,
    						SDL_WINDOW_FULLSCREEN_DESKTOP	//flags etc
    						);

    if (window==NULL)
    {
    	printf("Couldnt create window\n");
    }

    TTF_Init();
    TTF_Font
    SDL_Delay(8000); //8 sek


    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
	
}