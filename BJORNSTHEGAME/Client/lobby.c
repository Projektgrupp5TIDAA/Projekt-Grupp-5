
#ifndef _LOBBY_
#define _LOBBY_
#include <stdio.h>
#include <SDL2/SDL.h>
#include <string.h>
#ifdef __APPLE__
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <SDL2_net/SDL_net.h>
#else
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_net.h>
#endif
#include "bjornstartup.h"
#include <time.h> 
#include "lobby.h"
#endif


int LobbyWindow(StartInfo lobbyConnection){
    //********************** INIT *************************
    TTF_Font* playerfont= TTF_OpenFont("../Images/menu/coolvetica.ttf", 30);
    SDL_Window* lobby;
	SDL_Surface* lobbySurface;
    //background pic for lobby
	SDL_Surface* lobbyBackground = IMG_Load("../Images/lobby/lobby.png");
    // image for ready button
	SDL_Surface* readyButton = IMG_Load("../Images/lobby/ready.png");
    SDL_Rect buttonPlacement;
    SDL_Thread* timerfunc;
    
    nrecv name; // names struct

    // Struct for clock, see lobby.h
    clkInfo clockInfo;
    char packet[512];
    int i;
    //the music for the lobby
    Mix_Music *lobbyMusic = Mix_LoadMUS("../Sounds/Music/VolatileReaction.mp3");
    Mix_PlayMusic(lobbyMusic, -1);

    SDL_Rect player1;
    SDL_Rect player2;
    SDL_Rect player3;
    SDL_Rect player4;
    SDL_Rect player5;
    SDL_Rect player6;
    
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
                lobbySurface = SDL_GetWindowSurface( lobby );
                
            //rectangle for button
                buttonPlacement.x = (lobbySurface->w/3)-240;
                buttonPlacement.y = (lobbySurface->h/2)+200;
                buttonPlacement.w =  400;
                buttonPlacement.h =  80;
                
                player1.x=(lobbySurface->w/2)-370;
                player1.y=(lobbySurface->h/2)-50;
                player1.w=450;
                player1.h=400;
                
                player2.x=(lobbySurface->w/2)-370;
                player2.y=(lobbySurface->h/2)+25;
                player2.w=450;
                player2.h=400;
                
                player3.x=(lobbySurface->w/2)-370;
                player3.y=(lobbySurface->h/2)+125;
                player3.w=450;
                player3.h=400;
                
                player4.x=(lobbySurface->w/2)-90;
                player4.y=(lobbySurface->h/2)-50;
                player4.w=300;
                player4.h=400;
                
                player5.x=(lobbySurface->w/2)-90;
                player5.y=(lobbySurface->h/2)+25;
                player5.w=300;
                player5.h=400;
                
                player6.x=(lobbySurface->w/2)-90;
                player6.y=(lobbySurface->h/2)+125;
                player6.w=300;
                player6.h=400;
            }
        }
    }
       /* timerfunc = SDL_CreateThread(TimeThread, "TimeThread", (void*)&lobby);
            if(timerfunc==NULL)
            {
                fprintf(stderr, "Cant create thread for clock, %s\n", SDL_GetError());

            }*/
    


       /* if(*(lobbyConnection.socket)!= NULL){
            if(!(SDLNet_TCP_Send(*(lobbyConnection.socket), "N", 1))){
                printf("Could not connect to host: %s\n", SDLNet_GetError());
            }else{
                if(SDLNet_TCP_Recv(*(lobbyConnection.socket), packet, 512)){
                   memcpy(&name, &packet, sizeof(name));
                   printf("names struct includes now: %s\n", name.names[5]);
                }
            }
        }*/
    while(!endLobby){
        // Mouse events handling
        SDL_PumpEvents();
        SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);
        if(*(lobbyConnection.socket)!= NULL){
            if(!(SDLNet_TCP_Send(*(lobbyConnection.socket), "N", 1))){
                printf("Could not connect to host: %s\n", SDLNet_GetError());
            }//else{
            
            // }
                
        }
        if(SDLNet_TCP_Recv(*(lobbyConnection.socket), packet, 200)){
            if(packet[0]=='C')
                printf("%s\n",packet);
            else
                memcpy(&name, &packet, sizeof(name));
        }
        textToScreen(playerfont, player1, lobby, name.names[5]);
        textToScreen(playerfont, player2, lobby, name.names[4]);
        textToScreen(playerfont, player3, lobby, name.names[3]);
        textToScreen(playerfont, player4, lobby, name.names[2]);
        textToScreen(playerfont, player5, lobby, name.names[1]);
        textToScreen(playerfont, player6, lobby, name.names[0]);
        SDL_Delay(500);
        if( getMouseBounds(mousePosition, buttonPlacement))
        {
            if(SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) //leftclick
            {
                printf("PLAYER %s IS READY!\n", lobbyConnection.playerName);
                
                endLobby=1;
            }
        }
        
        SDL_BlitScaled(lobbyBackground, NULL, lobbySurface, NULL);
        SDL_BlitScaled(readyButton, NULL, lobbySurface, &buttonPlacement);
         //Update the surface
        SDL_UpdateWindowSurface(lobby);
    }
    printf("Shut down in progress\n");
    SDL_FreeSurface(lobbySurface);
    SDL_DestroyWindow(lobby); //Destroy window
    SDL_FreeSurface(lobbyBackground);
    SDL_FreeSurface(readyButton);

    SDL_Quit(); //Quit SDL subsystems
    return 0;
}

//a thread that updates time left in lobby.
 void TimeThread(SDL_Window* lobbyscreen){
    //få tid av server
    int time = 12;
    char time_in_text[3];

    SDL_Surface* window = SDL_GetWindowSurface(lobbyscreen);
    SDL_Rect clockPlace;
    SDL_Surface *textsurface;
            //rectangle for clock
            clockPlace.x = (window->w/3)+240;
            clockPlace.y = (window->h/2)-200;
            clockPlace.w =  400;
            clockPlace.h =  80;
    TTF_Font *clockFont = TTF_OpenFont("../Images/menu/StencilStd.ttf", 30);
    SDL_Colour fontcolour={0,0,0};

    for (int i = time; i > 0; --i)
    {
        printf("entering loop in thread\n");
        sprintf(time_in_text, "%d", i);
        printf("sprintf time_in_text\n");
        SDL_Delay(500);
        printf("delay is working in thread\n");
        textsurface = TTF_RenderText_Solid(clockFont, time_in_text, fontcolour);
        SDL_BlitSurface(textsurface, NULL, window, &clockPlace);
        SDL_Delay(500);
        printf("threadloop is working\n");
    }
    printf("GAME HAS STARTED\n");

   /*  clkInfo* changeclk = (clkInfo*) clockInfo;

    changeclk->seconds_left = 120;

    while(changeclk->seconds_left > 0)
    {
        changeclk->seconds_left = (changeclk->seconds_left) -1;
        SDL_Delay(995);
        sprintf((changeclk->sendingTime), "%d", (changeclk->seconds_left));
    }*/ 
 }




