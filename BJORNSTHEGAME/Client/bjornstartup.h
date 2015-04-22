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

int menu(SDL_Surface* screen, SDL_Window* window){
	SDL_Surface* background = IMG_Load("../Images/menu/MenuBack.png");
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_Music *music = Mix_LoadMUS("../Music/Mechanolith.mp3");
	Mix_VolumeMusic(64);
	Mix_PlayMusic(music, -1);
	SDL_Color black={0,0,0};
  	TTF_Font *font = TTF_OpenFont("../Images/menu/StencilStd.ttf", 120);
  	SDL_Surface *title = TTF_RenderText_Solid(font, "MENU", black);
  	SDL_Surface *helpbutton = TTF_RenderText_Solid(font, "HELP", black);
  	SDL_Surface *playbutton = IMG_Load("../Images/menu/MenuPlayButtonMin.png");
  	SDL_Surface *tapir = IMG_Load("../Images/menu/tapir_image.png");
	SDL_Rect titleplacement = {(screen->w/2 - ((title->w)/2)), 20, 0, 0};
	SDL_Rect buttonplacement = {((screen->w/2) - 125), 150, 250, 60};
	SDL_Rect tapirplacement = {0, (screen->h - 66), 100, 66};
  	SDL_BlitScaled(background, NULL, screen, NULL);
  	SDL_BlitSurface(title, NULL, screen, &titleplacement);
  	SDL_BlitScaled(playbutton, NULL, screen, &buttonplacement);
  	buttonplacement.y = 220;
  	SDL_BlitScaled(playbutton, NULL, screen, &buttonplacement);
  	buttonplacement.y = 290;
  	SDL_BlitScaled(helpbutton, NULL, screen, &buttonplacement);
  	SDL_BlitScaled(tapir, NULL, screen, &tapirplacement);
  	SDL_UpdateWindowSurface(window);
  	SDL_Delay(10000);
  	SDL_FreeSurface(title);
  	SDL_FreeSurface(helpbutton);
  	SDL_FreeSurface(playbutton);
  	SDL_FreeSurface(tapir);
  	Mix_FreeMusic(music);
  	TTF_CloseFont(font);
  	SDL_Delay(10000);
  	return 1;
}

#endif