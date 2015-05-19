#ifndef _ANIMATOR_H_
#define _ANIMATOR_H_

typedef struct{
	int frame, *quit, *ammo, *drunk;
	SDL_Window* window;
	playerInfo *player;
	playerInfo players[6];
    SDL_RendererFlip flip;
    bullet bullets[12];
    SDL_Rect platforms[14];
    SDL_Rect bjornDrapare[2];
}animationInfo;

int animate(void* updater);

void moveUP(int* test, SDL_Rect wall, SDL_Surface* s);

#endif