#ifndef HIT_H_INCLUDED
#define HIT_H_INCLUDED
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "player.h"

bool shut(){
  const Uint8 *state= SDL_GetKeyboardState(NULL);
  bool shooting= true;

  if(state[SDL_SCANCODE_X]==true ){
    shooting = true;
  }

  return shooting;
}

bool hit(){
  bool x = false;
  SDL_Rect position;
  Player p1,p2;
  position.x = p1.x;

  if(shut()==true && position.x ==p2.x){
    x = true;

 }
  return x;

}

#endif // HIT_H_INCLUDED
