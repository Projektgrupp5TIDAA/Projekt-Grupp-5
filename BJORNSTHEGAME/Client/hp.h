#ifndef _HP_HEADER_
#define _HP_HEADER_
#include <player.h>
void HP(Player *p, bool shutt)
{
	int newHP=0;
    if(shutt==true)
    {
        newHP= p->HP - p->drunkLevel ;
        p->HP = newHP;
        shutt=false;
    }
    if(p->HP==0)
    {
       //gameOver?
    }

}
#endif


