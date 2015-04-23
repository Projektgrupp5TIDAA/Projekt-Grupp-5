#ifndef AMMO_H_INCLUDED
#define AMMO_H_INCLUDED
#include <SDL2/SDL_mixer.h>
#define AMMO 3

int ammo(int newAmmo)
{

        newAmmo=AMMO-1;

        if(newAmmo == 0)
        {
            SDL_Delay(2000);//wait 2 sek

            //play sound effect while reloading
            Mix_Chunk *reload = MixLoadWav(".../Music/reload.wav");

            newAmmo += AMMO; //reload
        }

    return newAmmo;
}


#endif
