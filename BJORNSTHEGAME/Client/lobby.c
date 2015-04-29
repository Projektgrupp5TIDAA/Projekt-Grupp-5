#include "lobby.h"

int LobbyWindow(StartInfo lobbyConnection){

    //********************** INIT *************************
	SDL_Window* lobby;
	SDL_Surface* lobbySurface;
    //background pic for lobby
	SDL_Surface* lobbyBackground = IMG_Load("lobby.png");
    // image for ready button
	SDL_Surface* readyButton = IMG_Load("ready.png");
    SDL_Rect buttonPlacement;

    SDL_Thread* timerfunc;

    // Struct for clock, see lobby.h
    clkInfo clockInfo;

    //font for texts and clock
    TTF_Font *clockFont = TTF_OpenFont("../Images/menu/StencilStd.ttf", 30);
    SDL_Rect clockPlace;
    
    //the music for the lobby
    Mix_Music *lobbyMusic = Mix_LoadMUS("../Sounds/Music/VolatileReaction.mp3");
    Mix_PlayMusic(lobbyMusic, -1);

    timerfunc = SDL_CreateThread(TimeThread, "TimeThread", (void*)&clockInfo);
        if(timerfunc==NULL)
        {
            fprintf(stderr, "Cant create thread for clock, %s\n", SDL_GetError());
        }

    // mousePosition(X-axis,Y-axis)
    int mousePosition[2] = {0, 0};
    //gameloop
    int endLobby = 0;
    //*****************************************************


    //CHECK IF SDL IS NOT WORKING
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

            {
                    //Get window surface

            lobbySurface = SDL_GetWindowSurface( lobby );
 
            //rectangle for button
            buttonPlacement.x = (lobbySurface->w/3)-240;
            buttonPlacement.y = (lobbySurface->h/2)+200;
            buttonPlacement.w =  400;
            buttonPlacement.h =  80; 

            //rectangle for clock
            clockPlace.x = (lobbySurface->w/3)+240;
            clockPlace.y = (lobbySurface->h/2)-200;
            clockPlace.w =  400;
            clockPlace.h =  80;

        }
    }

    while(!endLobby){
        // Mouse events handling
        SDL_PumpEvents();
        SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);

        if((  SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)  ) )
        {
            if(  getMouseBounds(mousePosition, buttonPlacement) )
            {
                printf("PLAYER IS READY!\n");
                endLobby = 1;
            }
        }

        textToScreen(clockFont, clockPlace, lobby, clockInfo.sendingTime);

        SDL_BlitScaled(lobbyBackground, NULL, lobbySurface, NULL);
        SDL_BlitScaled(readyButton, NULL, lobbySurface, &buttonPlacement);
         //Update the surface
        SDL_UpdateWindowSurface(lobby);

    }  

    printf("Shut down in progress\n");
    SDL_DestroyWindow(lobby); //Destroy window
    
    SDL_FreeSurface(lobbySurface);
    SDL_FreeSurface(lobbyBackground);
    SDL_FreeSurface(readyButton);
        
    SDL_Quit(); //Quit SDL subsystems
    return 0;
}

//a thread that updates time left in lobby.
SDL_ThreadFunction* TimeThread(void* clockInfo){
    clkInfo* changeclk;
    changeclk = (clkInfo*) clockInfo;
    
    changeclk->seconds_left = 120;

    while(changeclk->seconds_left > 0)
    {
        changeclk->seconds_left = (changeclk->seconds_left) -1;
        SDL_Delay(990);
        sprintf((changeclk->sendingTime), "%d", (changeclk->seconds_left));
    }
    
}