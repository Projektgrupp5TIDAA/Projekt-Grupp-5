#include "bjornstartup.h"

int main(int argc, char *argv[])
{
  SDL_Window* window;
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
  window = SDL_CreateWindow("HEJ",				//Window name
			    SDL_WINDOWPOS_UNDEFINED,		//x-position
			    SDL_WINDOWPOS_UNDEFINED,		//y-position
			    800,
			    600,
                SDL_WINDOW_FULLSCREEN	//flags etc
			    );
  if (window==NULL)
    {
      printf("Window could not be created.\n");
    }
  
  SDL_Surface *screen = SDL_GetWindowSurface(window); // get the screen instead of using renderer function 
  menu(screen, window);
  SDL_Color white={255,255,255};
  TTF_Font *font = TTF_OpenFont("Metropolian-Text.ttf", 40);
  if(font == NULL){
    printf("Font can't be found or is in an unknown format.\n");
    exit(0);
  }
  SDL_Surface *text = TTF_RenderText_Solid(font, "I FIXED IT LOL", white);
  SDL_Surface *image = IMG_Load("yellow_player1.png");
  SDL_Surface *bg = IMG_Load("../Images/Backgrounds/Action-movie-Background1.jpg");
  SDL_Rect imagedestination = {0,50,45,75};
  SDL_BlitSurface(bg, NULL, screen, NULL);
  SDL_BlitScaled(image, NULL, screen, &imagedestination);
  SDL_BlitSurface(text, NULL, screen, NULL);
  SDL_UpdateWindowSurface(window);
  SDL_Delay(1000); //8 sek
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  TTF_Quit();
  return 0;
  
}
