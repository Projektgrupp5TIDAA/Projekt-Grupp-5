#include "lobby.h"

int getMouseBounds(int mouse[2], SDL_Rect rect);


void LobbyWindow(){
	SDL_Window* lobby;
	SDL_Surface* lobbySurface;
    //background pic for lobby
	SDL_Surface* lobbyBackground = IMG_Load("lobby.png");
    // image for ready button
	SDL_Surface* readyButton = IMG_Load("ready.png");
    
    SDL_Rect buttonPlacement = {((lobbySurface->w/3)-240),
                                ((lobbySurface->h/2)+200),
                                400,
                                80
                                };

    // mousePosition(X-axis,Y-axis)
    int mousePosition[2] = {0, 0};
    int endLobby = 0;

    while(!endLobby){
        
	    //Initialize SDL
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    	{
        	printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    	}

        else
        {
        //Create window
            lobby = SDL_CreateWindow(   "BJORNSLOBBY",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
        		  				        1280,
        	   	   				        800,
                                        SDL_WINDOW_FULLSCREEN);
            if( lobby == NULL )
            	{
                	printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            	}
    	    else
        	    {
                	//Get window surface
                	lobbySurface = SDL_GetWindowSurface( lobby );

				    SDL_BlitScaled(lobbyBackground, NULL, lobbySurface, NULL);

                    
                    //place ready button on screen
				    SDL_BlitScaled(readyButton, NULL, lobbySurface, &buttonPlacement);
            	
                    //Update the surface
                	SDL_UpdateWindowSurface(lobby);
            }
        }

        // Mouse events handling
        SDL_PumpEvents();
        SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);

        if(getMouseBounds(mousePosition, buttonPlacement))
        {
            if(  (  SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)  )  )
            {
                printf("PLAYER IS READY!\n");
                endLobby = 1;
            }
        }

    }  
    SDL_Delay(5000);

    printf("Shut down i progress\n");
    SDL_DestroyWindow(lobby); //Destroy window
    
    SDL_FreeSurface(lobbySurface);
    SDL_FreeSurface(lobbyBackground);
    SDL_FreeSurface(readyButton);

    SDL_Delay(3000);
        
    SDL_Quit(); //Quit SDL subsystems
}

int getMouseBounds(int mouse[2], SDL_Rect rect){
    if(mouse[0]>rect.x && mouse[0]<(rect.x+rect.w)){
        if(mouse[1]>rect.y && mouse[1]<(rect.y+rect.h)){
            return 1;
        }
    }
    return 0;
}