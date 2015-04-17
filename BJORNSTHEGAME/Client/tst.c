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
SDL_Rect position; 
struct player
{
    int x, y, dx;
    char name[WORDLENGTH];
    int plife;
    int shooting, walking, jumping;
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
    if(p.shooting ==0)
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
    if(p1.shooting==0 && p2.x==position.x)
    {
        n=true;
    }
    else
    {
        n=false;
    }


    return n;
}

int main(int argc, char const *argv[])
{

  bool shoots=false;
  Player p1= {10,20,15,"Martin",5,SHOOTING, WALKING,JUMPING,"yes"};//test
  shoots=shootedP();
  life(&p1, shoots);
  
  

    return 0;
}
