//
//  main.c
//  SDLtesta
//
//  Created by Jonas Wåhslen on 2015-02-16.
//  Copyright (c) 2015 wahslen. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void closeSDL();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

SDL_Surface* gXOut = NULL;

SDL_Renderer* gRenderer = NULL;

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    //Main loop flag
    bool quit = false;
    SDL_Rect dstrect;
    dstrect.x = 640/2;
    dstrect.y = 480/2;
    dstrect.w = 46;
    dstrect.h = 45;
    
    //Event handler
    SDL_Event e;
    
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Apply the image
            SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, &dstrect );
            //Update the surface
            SDL_UpdateWindowSurface( gWindow );
        }
    }
    
    //While application is running
    while( !quit){
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                printf( "QUIT!\n" );
                quit = true;
            }
            else if( e.type == SDL_KEYDOWN )
            {
                //Select surfaces based on key press
                switch( e.key.keysym.sym )
                {
                    case SDLK_UP:
                        dstrect.y -= 2;
                        break;
                    case SDLK_DOWN:
                        dstrect.y += 2;
                        break;
                    case SDLK_LEFT:
                        dstrect.x -= 2;
                        break;
                    case SDLK_RIGHT:
                        dstrect.x += 2;
                        break;
                }
                //Clear screen
               
                SDL_FillRect(gScreenSurface, NULL, 0x000000);
                SDL_Rect fillRect = { 100, 100, 20, 60 };
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF );
                SDL_RenderFillRect(gRenderer, &fillRect);
                SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, &dstrect);
                SDL_UpdateWindowSurface( gWindow );
            }
        }
        
        
    }
        //Apply the image
        SDL_BlitSurface( gXOut, NULL, gScreenSurface, &dstrect );
        
        //Update the surface
        SDL_UpdateWindowSurface( gWindow );
    
    IPaddress ip;
    TCPsocket sd;
    char serverip[20] = "";
    int port;
    printf("Server ip: "); scanf("%s", serverip);
    printf("Port: "); scanf("%d", &port);
    if (SDLNet_ResolveHost(&ip, serverip, port) < 0)
    {
        fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    if (!(sd = SDLNet_TCP_Open(&ip)))
    {
        fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    char *msg = "Testa";
    int len;
    
    len = (int) strlen(msg)+1;
    
    if (SDLNet_TCP_Send(sd, msg,len) < len)
    {
        fprintf(stderr, "SDLNet_TCP_Send: %s\n", SDLNet_GetError());
        exit(EXIT_FAILURE);
    }
    
    
        
        //Wait two seconds
        SDL_Delay( 1000 );
    
    
    
    
    
    //Free resources and close SDL
    closeSDL();

    return 0;
}

bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            gScreenSurface = SDL_GetWindowSurface( gWindow );
        }
    }
    
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load splash image
    gHelloWorld = SDL_LoadBMP("ball.bmp" );
    if( gHelloWorld == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", "ball.bmp", SDL_GetError() );
        success = false;
    }
    gXOut = SDL_LoadBMP("ball2.bmp");
    
    return success;
}

void closeSDL()
{
    //Deallocate surface
    SDL_FreeSurface( gHelloWorld );
    gHelloWorld = NULL;
    
    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    
    //Quit SDL subsystems
    SDL_Quit();
}
