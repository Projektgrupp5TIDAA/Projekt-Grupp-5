#ifndef _LOAD_WALKING_
#define _LOAD_WALKING_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#define PACKETSIZE 512
#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#define PLAYERCOUNT 6
#endif
//eventuella includes för alla OS
#include "loadwalking.h"
#endif

void show_walking(SDL_Rect position){



	return;
}

void loadMedia(){
    SDL_Surface* playerSurface = IMG_Load("spriteBlue.PNG");
    player = SDL_CreateTextureFromSurface(gRenderer, playerSurface);


    spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 210;
    spriteClips[0].h = 348;

    spriteClips[1].x = 210;
    spriteClips[1].y = 0;
    spriteClips[1].w = 210;
    spriteClips[1].h = 348;

    spriteClips[2].x = 420;
    spriteClips[2].y = 0;
    spriteClips[2].w = 210;
    spriteClips[2].h = 348;

    spriteClips[3].x = 630;
    spriteClips[3].y = 0;
    spriteClips[3].w = 210;
    spriteClips[3].h = 348;

}