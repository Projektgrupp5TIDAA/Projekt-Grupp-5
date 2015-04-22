#ifndef AMMO_H_INCLUDED
#define AMMO_H_INCLUDED
#define AMMO 3

int ammo(int newAmmo)
{

        newAmmo=AMMO-1;

        if(newAmmo == 0)
        {
            wait(2);//wait 2 sek

            //play sound effect when reloading

            newAmmo += AMMO; //reload
        }

    return newAmmo;
}


#endif
