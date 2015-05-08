
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
    const Uint8* keys;

    //surface for window
	SDL_Surface* lobbySurface;
    //background pic for lobby
	SDL_Surface* lobbyBackground = IMG_Load("../Images/lobby/lobby.png");
    // image for ready button
	SDL_Surface* readyButton = IMG_Load("../Images/lobby/ready.png");

    SDL_Rect buttonPlacement;
    SDL_Rect chat[5];
    SDL_Rect typing;

    nrecv name = {{0}, {{0}}}; // names struct

    SDLNet_SocketSet csock = SDLNet_AllocSocketSet(1);
    SDLNet_AddSocket(csock, *(lobbyConnection.socket));

    char packet[512] = {0};
    char tmp[5][512]={{0}};

    int i, timer=0;

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

                player[0].x=(lobbySurface->w/2)-370;
                player[0].y=(lobbySurface->h/2)-50;

                player[1].x=(lobbySurface->w/2)-370;
                player[1].y=(lobbySurface->h/2)+25;

                player[2].x=(lobbySurface->w/2)-370;
                player[2].y=(lobbySurface->h/2)+125;

                player[3].x=(lobbySurface->w/2)-90;
                player[3].y=(lobbySurface->h/2)-50;

                player[4].x=(lobbySurface->w/2)-90;
                player[4].y=(lobbySurface->h/2)+25;

                player[5].x=(lobbySurface->w/2)-90;
                player[5].y=(lobbySurface->h/2)+125;

                for(i=0;i<5;i++){
                    chat[i].x=(lobbySurface->w)/3 * 2 + 5;
                    chat[i].y=(lobbySurface->h)/5 * 4 - (50*(i+1));
                }

                typing.x=(lobbySurface->w)/3 * 2 + 5;
                typing.y=(lobbySurface->h)/5 * 4;
            }
        }
    }
    while(!endLobby){
        // Mouse events handling
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        SDLNet_CheckSockets(csock, 0);
        SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);
        emptyString(packet, strlen(packet));
        //printf("Time: %d\n", timer);
        if(SDLNet_SocketReady(*(lobbyConnection.socket))){
            SDLNet_TCP_Recv(*(lobbyConnection.socket), packet, 200);
            switch(packet[0]){
                case 'C':
                    parseString(packet,1,strlen(packet));
                    printf("%s\n",packet);
                    for(i=0;i<5;i++){
                        strcpy(tmp[i], tmp[i+1]);
                    }
                    strcpy(tmp[0], packet);
                    break;
                case 'N':
                    parseString(packet, 1, strlen(packet));
                    memcpy(&name, &packet, sizeof(name));
                    break;
                case 'T':
                    parseString(packet, 1, strlen(packet));
                    timer = atoi(packet);
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
        if(keys[SDL_SCANCODE_T]){
            char temp[sizeof(packet)];
            emptyString(packet, sizeof(packet));
            readKeyboardToWindow(typing, temp, sizeof(temp), lobby, lobbyBackground);
            printf("String: %s\n", temp);
            sprintf(packet, "C%s", temp);
            printf("String after parse: %s\n", packet);
            SDLNet_TCP_Send(*(lobbyConnection.socket), packet, strlen(packet));
        }

        SDL_BlitScaled(lobbyBackground, NULL, lobbySurface, NULL);
        SDL_BlitScaled(readyButton, NULL, lobbySurface, &buttonPlacement);
        for(i=0;i<5;i++){
            textToScreen(playerfont, chat[5-i], lobby, tmp[i]);
        }
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

void parseString(char* inc, int hops, int len){
    int i;
    if(hops>0){
        for(i=0;i<len;i++){
            *(inc+i) = *(inc+i+hops);
        }
    }else if(hops<0){
        for(i=len;i>0;i++){
            *(inc+i) = *(inc+i-hops);
        }
    }
}







