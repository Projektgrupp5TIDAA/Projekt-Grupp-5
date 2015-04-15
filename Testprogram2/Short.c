//
//  Short.c
//  SDLtesta
//
//  Created by Jonas Wåhslen on 2015-03-26.
//  Copyright (c) 2015 wahslen. All rights reserved.
//

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

//Screen dimension
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


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
SDL_Renderer* gRenderer = NULL;

int main( int argc, char* args[] )
{
    init();
    loadMedia();
    while( !quit )
    {
        eventHandler();
        animate();
      }
    closeMedia();
    return 0;
}

void animate(){
    SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
    SDL_FillRect(gScreenSurface, NULL, 0x000000);
    SDL_FillRect(gScreenSurface, &fillRect, 0x400000);
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
                    possion.y -= 2;
                    break;
                case SDLK_DOWN:
                    possion.y += 2;
                    break;
                case SDLK_LEFT:
                    possion.x -= 2;
                    break;
                case SDLK_RIGHT:
                    possion.x += 2;
                    break;
                default:
                    
                    break;
            }
        }
    }
}


void init()
{
    SDL_Init( SDL_INIT_VIDEO );
    gWindow = SDL_CreateWindow( "SDL Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    gScreenSurface = SDL_GetWindowSurface( gWindow );
}

void loadMedia()
{
    gBall = SDL_LoadBMP( "supermario.bmp" );
    possion.x = 640/2;
    possion.y = 480/2;
    possion.w = 46;
    possion.h = 45;
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
