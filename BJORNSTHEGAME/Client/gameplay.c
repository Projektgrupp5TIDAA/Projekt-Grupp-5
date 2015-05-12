#include "gameplay.h"
#include "clientthreads.h"
#include "animation.h"

int gameplayWindow(ClientInfo* information)
{
    updaterInfo updater = {NULL, &(information->socket), {{0, 0, {0, 0, 0, 0}}}};
    animate(updater);
    
    SDL_Quit();
    TTF_Quit();
    return 0;

}


int timeupdater(void * inc_time){
    int* tmp_timer= (int*) inc_time;
    printf("Timer thread uppdater started\n");
    SDL_Delay(1000);
    while(1){
        if((*(tmp_timer)) > 0){
            (*(tmp_timer))--;
            printf("Gameplay time: %s is ticking\n", *tmp_timer);
        }
        SDL_Delay(995);
    }
    return 0;
}


bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}


void moveP(SDL_Rect p, SDL_Rect wall, SDL_Surface* s)
{


    if((p.x<0)||( p.x + p.w > s->w/2 -590)|| checkCollision(p,wall))
    {

        p.x -= SPEEDx;
    }



}

void moveUP(SDL_Rect p, SDL_Rect wall, SDL_Surface* s)
{

    if((p.y<0)||( p.y + p.h > s->h/2 -200)|| checkCollision(p,wall))
    {
        p.y -=SPEEDy;
    }



}

