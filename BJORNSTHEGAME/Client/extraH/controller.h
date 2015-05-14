#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "player.h"

void Controller(Player *p)
{

    SDL_Event event;
    int xV =0; //xVelocity
    int yV =0; //yVelocity
    /*Game loop and Check for events*/
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        /*looking for keypress*/
        case SDL_KEYDOWN:
            /*Check the SDLKey values and chagne the coords*/
            switch(event.key.keysym.sym)
            {
            case SDLK_LEFT:
                xV=-1;
                break;
            case SDLK_RIGHT:
                xV=1;
                break;
            case SDLK_UP:
                yV=-1;
                break;
            case SDLK_DOWN:
                yV=1;
                break;

            default:
                break;
            }
            break;

        case SDL_KEYUP:
            switch(event.key.keysym.sym)
            {
            case SDLK_LEFT:
                if(xV < 0)
                {
                    xV=0;
                }
                break;
            case SDLK_RIGHT:
                if(xV > 0)
                {
                    xV=0;
                }
                break;
            case SDLK_UP:
                if(yV < 0)
                {
                    yV=0;
                }
                break;
            case SDLK_DOWN:
                if(yV > 0)
                {
                    yV=0;
                }
                break;
            default:
                break;

            }
            break;
        default:
            break;
        }



    }


    /*Update the position*/
    p-> position[0] = xV; //update x;
    p-> position[1]= yV;// update y


}


#endif


