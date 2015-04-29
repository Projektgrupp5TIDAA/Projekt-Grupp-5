#ifndef _BJORN_STARTUP_H_
#define _BJORN_STARTUP_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#define PACKETSIZE 512
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

typedef struct startUpInfo{
	TCPsocket* socket;
	IPaddress* targethost;
	char playerName[20];
}StartInfo;

int getMouseBounds(int mouse[2], SDL_Rect rect);

int menu(StartInfo startup);

int getName(char* name, int len, SDL_Window* window);

int getIP(IPaddress* targethost, SDL_Window* window);

int connectToHost(IPaddress* targethost, TCPsocket* socket);

int textToScreen(TTF_Font *font, SDL_Rect place, SDL_Window* window, char* text);

int readKeyboard(char* output, int len);

int readKeyboardToMenuWindow(char* output, int len, SDL_Window* window, SDL_Surface* bg);

int emptyString(char* incoming, int len);

#endif
