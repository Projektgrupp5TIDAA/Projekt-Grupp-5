#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
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
#include "lobby.h"

#define SPEEDx 10
#define SPEEDy 10

int gameplayWindow(ClientInfo* information);
bool checkCollision( SDL_Rect a, SDL_Rect b );

#endif // GAMEPLAY_H_INCLUDED

