#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_
#define AMMOAMOUNT 3
#define PLATFORMAMOUNT 14
#define TEXTAMOUNT 3
#define DRINKAMOUNT 2

typedef struct{
	int frame, *quit;
	SDL_Window* window;
	playerInfo* players[PLAYERCOUNT];
    brecv* bullets[PLAYERCOUNT]; 
    SDL_RendererFlip flip;
    SDL_Rect capsRect[AMMOAMOUNT];
    SDL_Rect platforms[PLATFORMAMOUNT];
    SDL_Rect bjornDrapare[DRINKAMOUNT];
    SDL_Rect textRect[TEXTAMOUNT];
    int* animateammo;
}animationInfo;

int animate(void* updater);

#endif