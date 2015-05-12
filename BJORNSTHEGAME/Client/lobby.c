
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

/* The Lobby function, spawns a pre-game window where you can see other players and chat with them */
int LobbyWindow(ClientInfo* lobbyConnection){
    //********************** INIT *************************
    TTF_Font* playerfont= TTF_OpenFont("../Images/menu/coolvetica.ttf", 30);
    TTF_Font* chatfont= TTF_OpenFont("../Images/menu/coolvetica.ttf", 18);
    TTF_Font* timerfont= TTF_OpenFont("../Images/menu/StencilStd.ttf", 70);
    SDL_Window* lobby;
    const Uint8* keys;

    //surface for window
	SDL_Surface* lobbySurface;
    //background pic for lobby
	SDL_Surface* lobbyBackground = IMG_Load("../Images/lobby/lobby.png");
    // image for ready button
	SDL_Surface* exitButton = IMG_Load("../Images/lobby/exit.png");

    SDL_Rect buttonPlacement;
    SDL_Rect chat[20], typing[2], timerpos;

    char* message, timechar[8];

    SDL_Thread * timethr;
    
    nrecv name = {{0}, {{0}}}; // names struct

    SDLNet_SocketSet csock = SDLNet_AllocSocketSet(1);
    SDLNet_AddSocket(csock, lobbyConnection->socket);

    char packet[512] = {0};
    char chatmessages[20][512]={{0}};

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

                timerpos.x = (lobbySurface->w/5-10);
                timerpos.y = (lobbySurface->h/5+10);

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

                for(i=0;i<20;i++){
                    chat[i].x=(lobbySurface->w)/3 * 2 + 5;
                    chat[i].y=(lobbySurface->h)/5 * 4 - (22*(i+1));
                }

                typing[1].x=(lobbySurface->w)/3 * 2 + 5;
                typing[1].y=(lobbySurface->h)/5 * 4 + 36;
                typing[0].x=(lobbySurface->w)/3 * 2 + 5;
                typing[0].y=(lobbySurface->h)/5 * 4 + 16;
            }
        }
    }

    timethr = SDL_CreateThread(timepoll, "Time", (void*)&timer);
    if(timethr == NULL){
        fprintf(stderr, "Could not create the timer thread: %s\n", SDL_GetError());
        return 1;
    }

    while(!endLobby){
        // Mouse events handling
        SDL_PumpEvents();
        keys = SDL_GetKeyboardState(NULL);
        SDLNet_CheckSockets(csock, 0);
        SDL_GetMouseState(&mousePosition[0], &mousePosition[1]);
        emptyString(packet, strlen(packet));

        /* If there is activity on the socket it is recieved and sorted depending on type */
        if(SDLNet_SocketReady(lobbyConnection->socket)){
            SDLNet_TCP_Recv(lobbyConnection->socket, packet, 200);
            switch(packet[0]){
                case 'C':
                    parseString(packet,1,strlen(packet));
                    for(i=19;i>0;i--){
                        strcpy(chatmessages[i], chatmessages[i-2]);
                    }
                    message = strtok(packet, "\n");
                    message = strtok(NULL, "\n");
                    strcpy(chatmessages[0], message);
                    emptyString(message, strlen(message));
                    strcpy(chatmessages[1], packet);
                    break;
                case 'N':
                    parseString(packet, 1, strlen(packet));
                    memcpy(&name, &packet, sizeof(name));
                    break;
                case 'T':
                    parseString(packet, 1, strlen(packet));
                    timer=atoi(packet);
                    break;
                default:
                    printf("Invalid package recieved!\n");
                    break;
            }
        }

        /* If the player clicks on the ready button their state will be set to ready */
        if( getMouseBounds(mousePosition, buttonPlacement))
        {
            if(SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) //leftclick
            {
                printf("%s exisited the lobby!\n", lobbyConnection->playerName);
                return 1;
            }
        }

        /* Checks if the 't'-key has been hit, in which case the keyboard is recorded to form a chat message */
        if(keys[SDL_SCANCODE_T]){
            char temp[sizeof(packet)];
            emptyString(packet, sizeof(packet));
            readKeyboardToWindow(chatfont, typing[1], temp, 32, lobby, lobbyBackground);
            sprintf(packet, "C%s says:\n   %s", lobbyConnection->playerName, temp);
            SDLNet_TCP_Send(lobbyConnection->socket, packet, strlen(packet));
        }

        /* Applies all pictures and text to the window */
        SDL_BlitScaled(lobbyBackground, NULL, lobbySurface, NULL);
        SDL_BlitScaled(exitButton, NULL, lobbySurface, &buttonPlacement);
        textToScreen(chatfont, typing[0], lobby, "Press 't' to type:");
        for(i=0;i<PLAYERCOUNT;i++){
            textToScreen(playerfont, player[i], lobby, name.names[5-i]);
        }

        for(i=0;i<19;i++){
            textToScreen(chatfont, chat[i], lobby, chatmessages[i]);
        }

        if(timer>0){
            convertTimer(timechar, timer);
            textToScreen(timerfont, timerpos, lobby, timechar);
        }else{
            textToScreen(chatfont, timerpos, lobby, "Waiting for more players.");
        }

        //Update the surface
        SDL_UpdateWindowSurface(lobby);
        SDL_Delay(20);
    }
    printf("Shut down in progress\n");
    SDL_FreeSurface(lobbySurface);
    SDL_DestroyWindow(lobby); //Destroy window
    SDL_FreeSurface(lobbyBackground);
    SDL_FreeSurface(exitButton);

    SDL_Quit(); //Quit SDL subsystems
    return 0;
}

/* Converts the current time from seconds into a string in 'MM:SS' format */
void convertTimer(char output[8], int input){
    int min=0;
    for(;input>59;input-=60){
        min++;
    }
    if(input>9)
        sprintf(output, "%d:%d", min, input);
    else
        sprintf(output, "%d:0%d", min, input);
}

/* Parses a string forwards or back 'hops' times */
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

/* Timer thread, uses delay to count down an int */
int timepoll(void* inctimer){
    int* timer = (int*) inctimer;
    printf("Timer thread started!\n");
    SDL_Delay(1000);
    while(1){
        if((*(timer)) > 0){
            (*(timer))--;
            printf("Time ticking: %d\n", *timer);
        }
        SDL_Delay(995);
    }
    return 0;
}



