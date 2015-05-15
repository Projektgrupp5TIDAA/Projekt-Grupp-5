#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_
#define AMMOAMOUNT 3
#define PLATFORMAMOUNT 14
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2

typedef struct{
	int frame, *quit, *ammo;
	SDL_Window* window;
	playerInfo players[PLAYERCOUNT];
    SDL_RendererFlip flip;
    bullet bullets[12];
    SDL_Rect capsRect[AMMOAMOUNT];
    SDL_Rect platforms[PLATFORMAMOUNT];
    SDL_Rect bjornDrapare[DRINKAMOUNT];
    SDL_Rect textRect[TEXTAMOUNT];
}animationInfo;

int animate(void* updater);

void moveUP(int* test, SDL_Rect wall, SDL_Surface* s);

#endif