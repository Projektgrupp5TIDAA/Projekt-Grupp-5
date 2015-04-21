//
//  Short.c
//  SDLtesta
//
//  Created by Jonas Wåhslen on 2015-03-26.
//  Copyright (c) 2015 wahslen. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdbool.h>
#define SPEED 10

//Screen dimension
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


void init();
void loadMedia();
void closeMedia();
void eventHandler();
void animate();
bool quit = false;
SDL_Rect possion;


SDL_Event e;                        //Event handler
SDL_Window* gWindow = NULL;         //The window we'll be rendering to
SDL_Surface* gScreenSurface = NULL; //The surface contained by the window
SDL_Surface* gBall = NULL;    //The image we will load and show on the screen
SDL_Surface* bg = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Rect fillRect = {0, 430, SCREEN_WIDTH, 200}; 

int main( int argc, char* argv[] )
{
  init();
  loadMedia();
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
  Mix_Music *music = Mix_LoadMUS("music2.mp3");
  Mix_VolumeMusic(128);
  Mix_PlayMusic(music, -1);
  while( !quit ){
    eventHandler();
    if(groundCollision() == 0){
      possion.y += 1;
    }
    animate();
  }
  closeMedia();
  return 0;
}

void animate(){
  SDL_BlitSurface(bg, NULL, gScreenSurface, NULL);
  SDL_FillRect(gScreenSurface, &fillRect, 0x000000);
  SDL_BlitSurface( gBall, NULL, gScreenSurface, &possion );
  SDL_UpdateWindowSurface( gWindow );
}

void eventHandler(){
  while( SDL_PollEvent( &e ) != 0 )
    {
      //User requests quit
      if( e.type == SDL_QUIT ){
	quit = true;
      }
      else if( e.type == SDL_KEYDOWN )
        {
	  //Select surfaces based on key press
	  switch( e.key.keysym.sym )
            {
	    case SDLK_UP:
	      if(groundCollision())
		possion.y -= SPEED+20;
	      break;
	    case SDLK_DOWN:
	      if(!groundCollision())
	      possion.y += SPEED;
	      break;
	    case SDLK_LEFT:
	      possion.x -= SPEED;
	      gBall = IMG_Load("blue_player1rev.png");
	      break;
	    case SDLK_RIGHT:
	      possion.x += SPEED;
	      gBall = IMG_Load("blue_player1.png");
	      break;
	    default:
	      
	      break;
            }
        }
    }
}

int groundCollision(){
  if(possion.y+150 > fillRect.y)
    return 1;
  return 0;
}

void init()
{
    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    gWindow = SDL_CreateWindow( "BJORNSTHEGAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    gScreenSurface = SDL_GetWindowSurface( gWindow );
}

void loadMedia()
{
    gBall = IMG_Load("blue_player1.png");
    possion.x = 0;
    possion.y = 0;
    possion.w = 91;
    possion.h = 150;
    bg = IMG_Load("bg.jpg");
    SDL_BlitSurface( gBall, NULL, gScreenSurface, &possion );
    SDL_UpdateWindowSurface( gWindow );
}

void closeMedia()
{
    SDL_FreeSurface( gBall );
    gBall = NULL;
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    
    SDL_Quit();
}
