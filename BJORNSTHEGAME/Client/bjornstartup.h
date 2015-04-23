#ifndef _BJORN_STARTUP_H_
#define _BJORN_STARTUP_H_
#include <stdio.h>
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#else
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif
/* information regarding the mouse state*/
/*typedef struct{
    Uint8 type;
    Uint8 state;
    Uint16 x, y;
    Sint16 xrel, yrel;
} SOL;*/

int getMouseBounds(int mouse[2], SDL_Rect rect);

int menu(SDL_Surface* screen, SDL_Window* window){
	int quit = 0, mouse[2] = {0};
	SDL_bool mouseinbounds = SDL_FALSE;
    // SDL_Event event;

	/* Load image-surfaces */
	SDL_Surface* background = IMG_Load("../Images/menu/MenuBack.png");
  	SDL_Surface* playbutton = IMG_Load("../Images/menu/MenuPlayButtonMin.png");
  	SDL_Surface* tapir = IMG_Load("../Images/menu/tapir_image.png");
  	SDL_Surface* helpbutton = IMG_Load("../Images/menu/door.png");

  	/* Load colour, font and then render text-surfaces */
	SDL_Colour black={0,0,0};
  	TTF_Font *font = TTF_OpenFont("../Images/menu/StencilStd.ttf", 120);
  	SDL_Surface *title = TTF_RenderText_Solid(font, "MENU", black);

  	/* Load music, set volume and start */
	Mix_Music *music = Mix_LoadMUS("../Music/Mechanolith.mp3");
	Mix_Chunk *uselt = Mix_LoadWAV("../Sounds/uselt.wav");
	Mix_VolumeMusic(64);
	Mix_PlayMusic(music, -1);

	/* Initialize rectangles */
	SDL_Rect titleplacement = {(screen->w/2 - ((title->w)/2)), 20, 0, 0}; // x,y,w,h
	SDL_Rect buttonplacement = {((screen->w/2) - 125), (screen->h/2), 250, 60};
	SDL_Rect button2placement = {((screen->w/2) - 125), (screen->h/2 + 70), 250, 60};
	SDL_Rect button3placement = {((screen->w/2) - 125), (screen->h/2 - 70), 250, 60};
	SDL_Rect helpplacement = {(screen->w - 126), (screen->h - 206), 120, 200};
	SDL_Rect tapirplacement = {0, (screen->h - 66), 100, 66};

	while(!quit){
		SDL_PumpEvents();
        /*while(SDL_PollEvent(&event)){ // track the mouse movements
            if(event.type == SDL_QUIT)
                quit=0;
            if(event.type == SDL_MOUSEMOTION){
                SDL_PixelFormat* fmt = screen ->format;
                if(event.motion.xrel <0)// mouse if the left
                    SDL_FillRect(screen, NULL, SDL_MapRGB(fmt, 255, 0,0));
                else if(event.motion.xrel >0) // moving to the right
                    SDL_FillRect(screen, NULL, SDL_MapRGB(fmt, 0 ,255, 0));
                else if(event.motion.yrel <0) // if moving up
                    SDL_FillRect(screen, NULL, SDL_MapRGB(fmt, 0, 0, 255));
                else if(event.motion.yrel >0) // down
                    SDL_FillRect(screen, NULL, SDL_MapRGB(fmt, 0, 255, 255));
            }
        }*/
        SDL_GetMouseState(&mouse[0], &mouse[1]); // get mouse state
		if(getMouseBounds(mouse, helpplacement)){
            if(SDL_GetMouseState(NULL, NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
            Mix_PlayChannel(-1, uselt, 1);
			SDL_Delay(4000);
            SDL_GetMouseState(&mouse[0], &mouse[1]);
			/* Free the used resources and return*/
  			SDL_FreeSurface(title);
  			SDL_FreeSurface(helpbutton);
  			SDL_FreeSurface(playbutton);
  			SDL_FreeSurface(tapir);
  			Mix_FreeMusic(music);
  			Mix_FreeChunk(uselt);
  			TTF_CloseFont(font);
			return 1;
            }
		}

		/* Blit the images to the screen */
  		SDL_BlitScaled(background, NULL, screen, NULL);
  		SDL_BlitSurface(title, NULL, screen, &titleplacement);
  		SDL_BlitScaled(playbutton, NULL, screen, &buttonplacement);
  		SDL_BlitScaled(playbutton, NULL, screen, &button2placement);
  		SDL_BlitScaled(playbutton, NULL, screen, &button3placement);
  		SDL_BlitScaled(helpbutton, NULL, screen, &helpplacement);
  		SDL_BlitScaled(tapir, NULL, screen, &tapirplacement);

  		/* Update the window */
  		SDL_UpdateWindowSurface(window);
  	}
  	SDL_Delay(10000);

  	return 1;
}

int getMouseBounds(int mouse[2], SDL_Rect rect){
    int quit=0;
    if(mouse[0]>rect.x && mouse[0]<(rect.x+rect.w)){
		if(mouse[1]>rect.y && mouse[1]<(rect.y+rect.h)){
            return 1;
        }
    }
    return 0;
}

#endif