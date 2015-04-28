#include "lobby.h"
void LobbyWindow(){
	SDL_Window* lobby;
	SDL_Surface* lobbySurface;
	SDL_Surface* lobbyBackground;
	SDL_Surface* readyButton;

	    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    	{
        	printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    	}

    else
    {
        //Create window
        lobby = SDL_CreateWindow( "BJORNSLOBBY",
        						SDL_WINDOWPOS_UNDEFINED,
        						SDL_WINDOWPOS_UNDEFINED,
        						1920,
        						1080,
        						SDL_WINDOW_FULLSCREEN);
        if( lobby == NULL )
        	{
            	printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        	}
    	else
        	{
            	//Get window surface
            	lobbySurface = SDL_GetWindowSurface( lobby );

                //load background pic to window
            	lobbyBackground = IMG_Load("lobby.png");
				SDL_BlitScaled(lobbyBackground, NULL, lobbySurface, NULL);

                //load the ready button
				readyButton = IMG_Load("ready.png");
                //place it on screen
				SDL_Rect buttonPlacement = {
                                            ((lobbySurface->w/3)-380),
                                            ((lobbySurface->h/2)+350),
                                            850,
                                            130
                                            };
				SDL_BlitScaled(readyButton, NULL, lobbySurface, &buttonPlacement);
            	
                //Update the surface
            	SDL_UpdateWindowSurface(lobby);
        }
    }

    SDL_Delay(5000);


    //Destroy window
    SDL_DestroyWindow(lobby);
    //Quit SDL subsystems
    SDL_Quit();
}
