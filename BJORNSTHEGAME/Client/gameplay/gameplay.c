#include "gameplay.h"

void gameplayWindow()
{
    int i;
    SDL_Window* gameplay;


    SDL_Surface* gameBackground = IMG_Load("bar.jpg");;
    SDL_Surface* ground =IMG_Load("ground2.png");
    SDL_Surface* platform1[35];
    SDL_Surface* platform2 = IMG_Load("platform_ver.png");
    SDL_Surface* band = IMG_Load("band2.png");
    SDL_Surface* bjrons = IMG_Load("bjorndrapare.png");
    SDL_Surface* ammo = IMG_Load("caps.png");
    SDL_Renderer* gRenderer = NULL;
    SDL_Texture* player = NULL;
    SDL_Surface* playerSurface = IMG_Load("spriteBlue.PNG");
    SDL_Rect platforms[30];
    SDL_Texture* bakgroundTexture;
    SDL_Texture* picture[30];
    SDL_Rect spriteClips[4];

    for(i=0;i<30;i++){
         platform1[i] = IMG_Load("platform_hor.png");
    }


    if(SDL_Init(SDL_INIT_VIDEO)<0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        TTF_Init();
        //Create a window
        gameplay = SDL_CreateWindow("BJORNSGAMEPLAY", SDL_WINDOWPOS_UNDEFINED,
                                    SDL_WINDOWPOS_UNDEFINED,
                                    1920,1080,
                                    SDL_WINDOW_FULLSCREEN_DESKTOP);
        if(gameplay == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        }
        else
        {
            SDL_Surface* screen = SDL_GetWindowSurface(gameplay);
            printf("Width: %d, Height: %d\n", screen->w, screen->h);



            gRenderer =SDL_CreateRenderer(gameplay, -1, SDL_RENDERER_ACCELERATED);
            bakgroundTexture= SDL_CreateTextureFromSurface(gRenderer,gameBackground);
            SDL_FreeSurface(gameBackground);

            player = SDL_CreateTextureFromSurface(gRenderer, playerSurface);
            SDL_FreeSurface(playerSurface);

            picture[0]= SDL_CreateTextureFromSurface(gRenderer,ground );
            SDL_FreeSurface(ground);

            picture[1]= SDL_CreateTextureFromSurface(gRenderer,band);
            SDL_FreeSurface(band);

            picture[2]= SDL_CreateTextureFromSurface(gRenderer,platform1[0]);
            SDL_FreeSurface(platform1[0]);

            picture[3]= SDL_CreateTextureFromSurface(gRenderer,platform2);
            SDL_FreeSurface(platform2);

            picture[4]= SDL_CreateTextureFromSurface(gRenderer,platform1[1]);
            SDL_FreeSurface(platform1[1]);

            picture[5]= SDL_CreateTextureFromSurface(gRenderer,platform1[2]);
            SDL_FreeSurface(platform1[2]);

            picture[6]= SDL_CreateTextureFromSurface(gRenderer,platform1[3]);
            SDL_FreeSurface(platform1[3]);

            picture[7]= SDL_CreateTextureFromSurface(gRenderer,platform1[4]);
            SDL_FreeSurface(platform1[4]);

            picture[8]= SDL_CreateTextureFromSurface(gRenderer,platform1[5]);
            SDL_FreeSurface(platform1[5]);

            picture[9]= SDL_CreateTextureFromSurface(gRenderer,platform1[6]);
            SDL_FreeSurface(platform1[6]);

            picture[10]= SDL_CreateTextureFromSurface(gRenderer,platform1[7]);
            SDL_FreeSurface(platform1[7]);

            picture[11]= SDL_CreateTextureFromSurface(gRenderer,platform1[8]);
            SDL_FreeSurface(platform1[8]);

            picture[12]= SDL_CreateTextureFromSurface(gRenderer,platform1[9]);
            SDL_FreeSurface(platform1[9]);

            picture[13]= SDL_CreateTextureFromSurface(gRenderer,platform1[10]);
            SDL_FreeSurface(platform1[10]);




            platforms[0].x = screen->w/5 -300;
            platforms[0].y =screen ->h -65;
            platforms[0].w = 1920;
            platforms[0].h =70;

            platforms[1].x = screen->w/5 -300;
            platforms[1].y =screen ->h/2 -390;
            platforms[1].w = 1920;
            platforms[1].h = 90;

            platforms[2].x = screen->w/3 -200;
            platforms[2].y = screen->h - 180;
            platforms[2].w = 550;
            platforms[2].h = 40;

            platforms[3].x = screen->w/3 +30;
            platforms[3].y = screen->h -590;
            platforms[3].w = 45;
            platforms[3].h = 410;

            platforms[4].x = screen ->w/3 -130;
            platforms[4].y = screen  ->h - 610;
            platforms[4].w = 350;
            platforms[4].h = 40;

            platforms[5].x = screen->w/3 -460;
            platforms[5].y = screen->h/2 -100;
            platforms[5].w = 150;
            platforms[5].h = 30;

            platforms[6].x = screen->w/3 -460;
            platforms[6].y = screen->h/2 +140;
            platforms[6].w = 150;
            platforms[6].h = 30;

            platforms[7].x = screen ->w/2 +300;
            platforms[7].y = screen->h/2 -80;
            platforms[7].w = 400;
            platforms[7].h = 60;

            platforms[8].x = screen->w/3 -139;
            platforms[8].y = screen -> h/2 -70;
            platforms[8].w = 170;
            platforms[8].h = 30;

            platforms[9].x = screen->w/3 -139;
            platforms[9].y = screen -> h/2 +90;
            platforms[9].w = 170;
            platforms[9].h = 30;

            platforms[10].x = screen ->w/3 +75;
            platforms[10].y = screen -> h/2 -70;
            platforms[10].w = 170;
            platforms[10].h = 30;

            platforms[11].x = screen ->w/3 +300;
            platforms[11].y = screen -> h/2 -140;
            platforms[11].w = 170;
            platforms[11].h = 30;

            platforms[12].x = screen ->w/2 +390;
            platforms[12].y = screen -> h/2 +150;
            platforms[12].w = 300;
            platforms[12].h = 40;

            platforms[13].x = screen ->w/3 +73;
            platforms[13].y = screen -> h/2 +30;
            platforms[13].w = 170;
            platforms[13].h = 30;


            SDL_Event e;
            SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
            bool quit = false;
            bool onPlatform = true;
            //size and position
            SDL_Rect position;
            position.y = screen-> h/2 +100;
            position.x = screen->w/2 +80;
            position.h = 100;
            position.w = 110;

            //start frame and be facing to the right
            int frame = 2;
            flip = SDL_FLIP_NONE;
            //float speed = 0;

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


            while (!quit)
            {
                while (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    if(e.type == SDL_KEYDOWN)
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

                            if(frame == 2)
                            {
                                frame = 0;
                            }
                            else if(frame == 0)
                            {
                                frame = 3;
                            }
                            else if(frame == 3)
                            {
                                frame = 1;
                            }
                            else
                            {
                                frame = 2;
                            }
                            break;

                        case SDLK_RIGHT:
                            position.x += SPEEDx;
                            flip = SDL_FLIP_NONE;
                            //speed += 1;

                            if(frame == 2)
                            {
                                frame = 0;
                            }
                            else if(frame == 0)
                            {
                                frame = 3;
                            }
                            else if(frame == 3)
                            {
                                frame = 1;
                            }
                            else
                            {
                                frame = 2;
                            }

                            break;
                        case SDLK_SPACE:
                            if(onPlatform == true)
                            {
                                onPlatform = false;
                            }


                            break;

                        default:

                            break;
                        }


                    }

                }







                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer,  bakgroundTexture,NULL,NULL);

                for(i=0;i<15;i++){
                   SDL_RenderCopy(gRenderer, picture[i],NULL,&platforms[i]);
                }

                SDL_RenderCopyEx(gRenderer, player, &spriteClips[frame],&position, 0, NULL, flip);


                SDL_RenderPresent(gRenderer);

            }



        }

    }



    //Destroy window
    SDL_DestroyWindow(gameplay);
    //Quit SDL subsystems


    for(i=0;i<15;i++){
        SDL_DestroyTexture(picture[i]);
    }

    SDL_DestroyTexture(player);
    SDL_DestroyRenderer(gRenderer);


    SDL_Quit();
    TTF_Quit();

}




