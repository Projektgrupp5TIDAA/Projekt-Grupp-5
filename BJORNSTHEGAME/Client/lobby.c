#include "lobby.h"

int LobbyWindow(StartInfo lobbyConnection){
    //********************** INIT *************************
	char packet[512];
    SDL_Window* lobby;
	SDL_Surface* lobbySurface;
    //background pic for lobby
	SDL_Surface* lobbyBackground = IMG_Load("../Images/lobby/lobby.png");
    // image for ready button
	SDL_Surface* readyButton = IMG_Load("../Images/lobby/ready.png");
    SDL_Rect buttonPlacement;
    SDL_Rect player1;

    SDL_Thread* timerfunc;

    // Struct for clock, see lobby.h
    clkInfo clockInfo;
    

    //the music for the lobby
    Mix_Music *lobbyMusic = Mix_LoadMUS("../Sounds/Music/VolatileReaction.mp3");
    Mix_PlayMusic(lobbyMusic, -1);

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

            // rectangle for playername
                player1.x=(lobbySurface->w/3)+150;
                player1.y=(lobbySurface->h/2)-150;
                player1.w=350;
                player1.h=70;

            }
        }
        timerfunc = SDL_CreateThread(TimeThread, "TimeThread", (void*)lobby);
            if(timerfunc==NULL)
            {
                fprintf(stderr, "Cant create thread for clock, %s\n", SDL_GetError());
            }
    }

    while(!endLobby){
        // Mouse events handling
        SDL_PumpEvents();
        SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);
        
        /* get how many players in the lobby and determine which place the playername will showup on the screen - not done yet */
        *(lobbyConnection.socket)=SDLNet_TCP_Open(lobbyConnection.targethost);
        if(*(lobbyConnection.socket) != NULL){
            if(!(SDLNet_TCP_Send(*(lobbyConnection.socket), "I", 1))){
                printf("Could not connect to host: %s\n", SDLNet_GetError());
            }else{
                while(1){
                    if(SDLNet_TCP_Recv(*(lobbyConnection.socket),packet, 512)){
                        printf("Got the info: %s\n", packet);
                        // if any player disconnects = problem
                        if(strstr("Server - 1/6", packet)){
                            // rectangle player1
                            textToScreen(clockFont, player1, lobby, packet);
                            break;
                        }
                        else if(strstr("Server - 2/6", packet)){
                            // rectangle player2
                            break;
                        }
                        else if(strstr("Server - 3/6", packet)){
                            // rectangle player3
                            break;
                        }
                        else if(strstr("Server - 4/6", packet)){
                            // rectangle player4
                            break;
                        }
                        else if(strstr("Server - 5/6", packet)){
                            // rectangle player5
                            break;
                        }
                        else if(strstr("Server - 6/6", packet)){
                            // rectangle player6
                            break;
                        }
                        // textToScreen(clockFont, player1, lobby, lobbyConnection.playerName);
                        // SDL_UpdateWindowSurface(lobby);
                    }else SDL_Delay(15);
                }
            }
        }
        
        if( getMouseBounds(mousePosition, buttonPlacement))
        {
            if(SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) //leftclick
            {
                printf("PLAYER IS READY!\n");
                /* go to the game window when time is over - not written yet */
                endLobby = 1;
            }
        }

       textToScreen(clockFont, clockPlace, lobby, clockInfo.sendingTime);

        //textToScreen(clockFont, clockPlace, lobby, clockInfo.sendingTime);


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
SDL_ThreadFunction* TimeThread(SDL_Window* screen)
{
    //få tid av server
    int time = 120;
    char time_in_text[3];

    SDL_Surface* window = SDL_GetWindowSurface(screen);
    SDL_Rect clockPlace;

            //rectangle for clock
            clockPlace.x = (window->w/3)+240;
            clockPlace.y = (window->h/2)-200;
            clockPlace.w =  400;
            clockPlace.h =  80;
    TTF_Font *font = TTF_OpenFont("../Images/menu/StencilStd.ttf", 30);


    for (int i = time; i > 0; --i)
    {
        sprintf(time_in_text, "%d", i);
        SDL_Delay(1000);
        textToScreen(font, clockPlace, screen, time_in_text);
    }
    printf("GAME HAS STARTED\n");







    /*clkInfo* changeclk = (clkInfo*) clockInfo;

    changeclk->seconds_left = 120;

    while(changeclk->seconds_left > 0)
    {
        changeclk->seconds_left = (changeclk->seconds_left) -1;
        SDL_Delay(995);
        sprintf((changeclk->sendingTime), "%d", (changeclk->seconds_left));
    }*/

}



