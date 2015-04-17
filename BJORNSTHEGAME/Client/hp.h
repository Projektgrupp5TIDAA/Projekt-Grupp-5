#ifndef _HP_HEADER_
#define _HP_HEADER_
#include <bjornsthreads.h>
void HP(pinfo *p, bool shutt)
{
	int newHP=0;
    if(shutt==true)
    {
        newHP= p->health - p->drunkLevel ;
        p->health = newHP;
        shutt=false;
    }
    if(p->health==0)
    {
       //gameOver?
    }

}
#endif


