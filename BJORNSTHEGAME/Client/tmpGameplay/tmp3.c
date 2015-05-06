#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#define SPEEDx 10

bool init();
void loadMedia();

SDL_Window* gameplay = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* player = NULL;
SDL_Surface* gameBackground = NULL;
SDL_Surface* gameSurface = NULL;

SDL_Rect spriteClips[4];

int main(int argc, char* args[])
{
    SDL_Event e;
    SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
    bool quit = false;
    bool onPlatform = true;
    //size and position
    SDL_Rect position;
    position.y = 700;
    position.x = 100;
    position.h = 108;
    position.w = 110;

    //start frame and be facing to the right
    int frame = 2/*,velocity_x, velocity_y*/;
    flip = SDL_FLIP_NONE;
    //float ySpeed = 0;

    if (init()) {
        printf("worked\n");
    }
    loadMedia();

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if(e.type == SDL_KEYDOWN)
            {
                //Select surfaces based on key press
                switch(e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    case SDLK_LEFT:
                        position.x -= SPEEDx;
                        flip = SDL_FLIP_HORIZONTAL;

                        if(frame == 2){
                            frame = 0;
                        }
                        else if(frame == 0){
                            frame = 3;
                        }
                        else if(frame == 3){
                            frame = 1;
                        }
                        else{
                            frame = 2;
                        }
                        break;

                    case SDLK_RIGHT:
                        position.x += SPEEDx;
                        flip = SDL_FLIP_NONE;
                        //speed += 1;

                        if(frame == 2){
                            frame = 0;
                        }
                        else if(frame == 0){
                            frame = 3;
                        }
                        else if(frame == 3){
                            frame = 1;
                        }
                        else{
                            frame = 2;
                        }

                        break;

                    case SDLK_UP:
                        /*if(onPlatform == true){
                                onPlatform = false;
                        }
                        position.y -=10;*/

                        /*speed -= 10;
                        position.y = position.y + 0.1*speed;
                        speed = speed - 0.1*speed;*/
                        break;

                    case SDLK_SPACE:
                        /*fire*/
                        printf("PEW ");
                        break;

                    default:

                        break;
                }

            }
            /*if(onPlatform == false){
                    position.y -= 5*ySpeed;
                    ySpeed -= 0.1*ySpeed;
                    position.y += 0.1*ySpeed;
                    //position.y -= 10;
                    onPlatform = true;
                }*/

        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        SDL_RenderCopyEx(gRenderer, player, &spriteClips[frame],&position , 0, NULL, flip);
        SDL_RenderPresent(gRenderer);

    }

    return 0;
}

void loadMedia(){
    /*gameSurface = SDL_GetWindowSurface(gameplay);
    SDL_BlitScaled(gameBackground,NULL,gameSurface,NULL);
    SDL_UpdateWindowSurface(gameplay);*/
    SDL_Surface* playerSurface = IMG_Load("spriteBlue.PNG");
    player = SDL_CreateTextureFromSurface(gRenderer, playerSurface);


    spriteClips[0].x = 0;
    spriteClips[0].y = 0;
    spriteClips[0].w = 210;
    spriteClips[0].h = 348;

    spriteClips[1].x = 210;
    spriteClips[1].y = 0;
    spriteClips[1].w = 210;
    spriteClips[1].h = 348;

    spriteClips[2].x = 420;
    spriteClips[2].y = 0;
    spriteClips[2].w = 210;
    spriteClips[2].h = 348;

    spriteClips[3].x = 630;
    spriteClips[3].y = 0;
    spriteClips[3].w = 210;
    spriteClips[3].h = 348;

}

bool init(){
    bool test = true;
    SDL_Init(SDL_INIT_VIDEO);
    gameplay = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_FULLSCREEN);
    if(gameplay == NULL){
        printf("Something went wrong\n");
        test = false;
    }
    gRenderer = SDL_CreateRenderer(gameplay, -1, SDL_RENDERER_ACCELERATED);
    if(gRenderer == NULL){
        printf("Something went wrong\n");
        test = false;
    }
    return test;

}


