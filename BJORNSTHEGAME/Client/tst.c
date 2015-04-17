#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#define WORDLENGTH 31
#define BULLETS 100
#define HP 5
#define SHOOTING 0
#define WALKING 1
#define JUMPING 2
#define Timer 10
SDL_Rect position;
struct player
{
    int x, y, dy;
    char name[WORDLENGTH];
    int plife;
    int shooting, walking, jumping, current, turningLeft;
    char alive[WORDLENGTH];

};

typedef struct player Player;
void life(Player *p, bool shooted)// counting HP (players life)
{
    int newHP=0;
    if(shooted==true)
    {
        newHP= HP ;//- drunkLevel()
        p->plife = newHP;
        shooted=false;
    }
    if(HP==0)
    {
        //gameOver?
        strcpy(p-> alive, "No");
    }
    else
    {
        p->plife = HP;
    }
}

int Bullets(Player p)//how many bullets left to the player?
{

    int newBullets=0;
    if(p.shooting ==1)
    {
        newBullets= BULLETS-1;
    }

    return newBullets;
}

bool shootedP()
{

    bool n=false;
    Player p1, p2;
    position.x=p1.x;
    if(p1.shooting==1 && p2.x==position.x)
    {
        n=true;
    }
    else
    {
        n=false;
    }


    return n;
}

int Events(SDL_Window *window, Player *p)
{
    SDL_Event event;
    int done = 0;

    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if(window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                break;
            }
        }
        break;
        case SDL_QUIT:
            //out of the game
            done = 1;
            break;
        }
    }

    const Uint8 *s = SDL_GetKeyboardState(NULL);
    if(!p->shooting)
    {
        if(s[SDL_SCANCODE_LEFT])
        {
            p->x -= 3;
            p->walking = 1;
            p->turningLeft = 1;

            if(Timer % 10 == 0)
            {
                p->current++;
                p->current %= 5;
            }
        }
        else if(s[SDL_SCANCODE_RIGHT])
        {
            p->x += 3;
            p->walking = 1;
            p->turningLeft = 0;

            if(Timer % 10 == 0)
            {
                p->current++;
                p->current %= 5;
            }
        }
        else
        {
            p->walking = 0;
            p->current = 5;
        }
    }

    if(!p->walking)
    {
        if(s[SDL_SCANCODE_SPACE])
        {
            if(Timer %10== 0)
            {
                if(p->current == 5)
                    p->current = 6;
                else
                    p->current = 5;

                if(!p->turningLeft)
                {

                    p->shooting = 1;

                }
            }

        }
        else
        {
            p->current = 5;
            p->shooting = 0;
        }
    }

    if(s[SDL_SCANCODE_UP] && !p->dy)
    {
        p->dy = -8;
    }
    if(s[SDL_SCANCODE_DOWN])
    {
        p->y += 10;
    }

    return done;
}

int main(int argc, char const *argv[])
{

    /* bool shoots=false;
    Player p1= {10,20,15,"Martin",5,SHOOTING, WALKING,JUMPING,"yes"};//test
    shoots=shootedP();
    life(&p1, shoots);*/



    return 0;
}
