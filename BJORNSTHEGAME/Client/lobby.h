#ifndef _LOBBY_
#define _LOBBY_
#include <stdio.h>
#include <SDL2/SDL.h>
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
int LobbyWindow(StartInfo lobbyConnection);
typedef struct clockInformation
{
	/*unsigned int time_milliseconds;
    unsigned int time_seconds;
    
    unsigned int countdown_secs, time_left, startTime;
    //checks if seconds has changed
    unsigned int check;*/
    int seconds_left;
    char sendingTime[3];

}clkInfo;
SDL_ThreadFunction* TimeThread(void* clockInfo);

#endif