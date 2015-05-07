
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
#define PLAYERCOUNT 6
#endif


int LobbyWindow(StartInfo lobbyConnection){
    //********************** INIT *************************
    TTF_Font* playerfont= TTF_OpenFont("../Images/menu/coolvetica.ttf", 30);
    SDL_Window* lobby;

    //surface for window
	SDL_Surface* lobbySurface;
    //background pic for lobby
	SDL_Surface* lobbyBackground = IMG_Load("../Images/lobby/lobby.png");
    // image for ready button
	SDL_Surface* readyButton = IMG_Load("../Images/lobby/ready.png");

    SDL_Rect buttonPlacement;
    SDL_Rect chat;

    nrecv name = {{0}, {{0}}}; // names struct

    SDLNet_SocketSet csock = SDLNet_AllocSocketSet(1);
    SDLNet_AddSocket(csock, *(lobbyConnection.socket));

    char packet[512] = {0};
    char tmp[512]={0};

    int i;

    //the music for the lobby
    Mix_Music *lobbyMusic = Mix_LoadMUS("../Sounds/Music/VolatileReaction.mp3");
    Mix_PlayMusic(lobbyMusic, -1);

    SDL_Rect player[6];

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
                                    0);
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

                player[0].x=(lobbySurface->w/2)-370;
                player[0].y=(lobbySurface->h/2)-50;
                player[0].w=450;
                player[0].h=400;

                player[1].x=(lobbySurface->w/2)-370;
                player[1].y=(lobbySurface->h/2)+25;
                player[1].w=450;
                player[1].h=400;

                player[2].x=(lobbySurface->w/2)-370;
                player[2].y=(lobbySurface->h/2)+125;
                player[2].w=450;
                player[2].h=400;

                player[3].x=(lobbySurface->w/2)-90;
                player[3].y=(lobbySurface->h/2)-50;
                player[3].w=300;
                player[3].h=400;

                player[4].x=(lobbySurface->w/2)-90;
                player[4].y=(lobbySurface->h/2)+25;
                player[4].w=300;
                player[4].h=400;

                player[5].x=(lobbySurface->w/2)-90;
                player[5].y=(lobbySurface->h/2)+125;
                player[5].w=300;
                player[5].h=400;
                
                
                chat.x=(lobbySurface->w/2)-370;
                chat.y=(lobbySurface->h/2)+125;
                chat.w=450;
                chat.h=400;
            }
        }
    }
    while(!endLobby){
        // Mouse events handling
        SDL_PumpEvents();
        SDLNet_CheckSockets(csock, 0);
        SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);
        if(SDLNet_SocketReady(*(lobbyConnection.socket))){
            SDLNet_TCP_Recv(*(lobbyConnection.socket), packet, 200);
            switch(packet[0]){
                case 'C':
                    parseChat(packet,1,strlen(packet));
                    printf("%s\n",packet);
                    strcpy(tmp, packet);
                    break;
                case 'N':
                    parseChat(packet, 1, strlen(packet));
                    memcpy(&name, &packet, sizeof(name));
                    break;
                default:
                    printf("Invalid package recieved!\n");
                    break;
            }
        }

        for(i=0;i<PLAYERCOUNT;i++){
            textToScreen(playerfont, player[i], lobby, name.names[5-i]);
        }

        //SDL_Delay(500);

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
        textToScreen(playerfont, chat, lobby, tmp);
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

void parseChat(char* inc, int hops, int len){
    int i;
    if(hops>0){
        for(i=0;i<len;i++){
            *(inc+i) = *(inc+i+hops);
        }
    }else if(hops<0){
        for(i=len-1;i>0;i++){
            *(inc+i) = *(inc+i-hops);
        }
    }
}







