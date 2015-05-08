#include "gameplay.h"

void gameplayWindow()
{
    int i;
    int size1=14, size2=3, size3=2;

    SDL_Window* gameplay;


    SDL_Surface* gameBackground = IMG_Load("bar.jpg");;
    SDL_Surface* ground =IMG_Load("ground2.png");
    SDL_Surface* platform1[size1];
    SDL_Surface* platform2 = IMG_Load("platform_ver.png");
    SDL_Surface* band = IMG_Load("band2.png");
    SDL_Surface* bjorns[size3];
    SDL_Surface* ammo[size2];
    SDL_Renderer* gRenderer = NULL;
    SDL_Texture* player = NULL;
    SDL_Surface* playerSurface = IMG_Load("spriteBlue.PNG");
    SDL_Rect platforms[size1];
    SDL_Texture* bakgroundTexture;
    SDL_Texture* picture[size1];
    SDL_Rect spriteClips[4];

    SDL_Texture* caps[size2];
    SDL_Rect capsRect[size2];

    SDL_Texture* bjornDrapare[size3];
    SDL_Rect bjornDRect[size3];

    SDL_Surface* textSurface[size2];
    SDL_Rect textRect[size2];
    SDL_Texture* myText[size2];

    for(i=0; i<size2; i++)
    {
        ammo[i]=IMG_Load("caps.png");
    }

    for(i=0; i<size3; i++)
    {
        bjorns[i]=IMG_Load("bjorndrapare.png");
    }

    for(i=0; i<size1; i++)
    {
        platform1[i] = IMG_Load("platform_hor.png");
        SDL_FillRect(platform1[i], NULL, SDL_MapRGB(platform1[i]->format, 200, 190, 200));
    }

     SDL_FillRect(platform2, NULL, SDL_MapRGB(platform2->format, 200, 190, 200));


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

            /*text*/
            SDL_Color colorT= {170,60,255};
            TTF_Font *font = TTF_OpenFont("StencilStd.ttf", 35);
            if(font == NULL)
                {printf("FONT E NULL\n");}

                textSurface[0]= TTF_RenderText_Solid(font, "Ammo:", colorT);
                textSurface[1]= TTF_RenderText_Solid(font, "HP:", colorT);
                textSurface[2]= TTF_RenderText_Solid(font, "Drunk level:", colorT);




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

            caps[0]= SDL_CreateTextureFromSurface(gRenderer,ammo[0]);
            SDL_FreeSurface(ammo[0]);

            caps[1]= SDL_CreateTextureFromSurface(gRenderer,ammo[1]);
            SDL_FreeSurface(ammo[1]);

            caps[2]= SDL_CreateTextureFromSurface(gRenderer,ammo[2]);
            SDL_FreeSurface(ammo[2]);

            bjornDrapare[0]=SDL_CreateTextureFromSurface(gRenderer,bjorns[0]);
            SDL_FreeSurface(bjorns[0]);

            bjornDrapare[1]=SDL_CreateTextureFromSurface(gRenderer,bjorns[1]);
            SDL_FreeSurface(bjorns[1]);

            myText[0]=SDL_CreateTextureFromSurface(gRenderer,textSurface[0]);
            SDL_FreeSurface(textSurface[0]);

            myText[1]=SDL_CreateTextureFromSurface(gRenderer,textSurface[1]);
            SDL_FreeSurface(textSurface[1]);

            myText[2]=SDL_CreateTextureFromSurface(gRenderer,textSurface[2]);
            SDL_FreeSurface(textSurface[2]);










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


            capsRect[0].x = screen ->w/2 +600;
            capsRect[0].y = screen ->h - 740;
            capsRect[0].w = 60;
            capsRect[0].h = 50;

            capsRect[1].x = screen ->w/2 +530;
            capsRect[1].y = screen ->h - 740;
            capsRect[1].w = 60;
            capsRect[1].h = 50;

            capsRect[2].x = screen ->w/2 +460;
            capsRect[2].y = screen ->h - 740;
            capsRect[2].w = 60;
            capsRect[2].h = 50;

            bjornDRect[0].x = screen  ->w/2 -110;
            bjornDRect[0].y = screen ->h - 228;
            bjornDRect[0].w = 60;
            bjornDRect[0].h = 50;

            bjornDRect[1].x = screen  ->w/2 +400;
            bjornDRect[1].y = screen ->h - 512;
            bjornDRect[1].w = 60;
            bjornDRect[1].h = 50;

            textRect[0].x= screen ->w/2 +310;
            textRect[0].y= screen ->h - 740;
            textRect[0].w= 120;
            textRect[0].h= 60;

            textRect[1].x= screen ->w/2 -660;
            textRect[1].y= screen ->h - 740;
            textRect[1].w= 90;
            textRect[1].h= 60;

            textRect[2].x= screen ->w/2 -450;
            textRect[2].y= screen ->h - 740;
            textRect[2].w= 230;
            textRect[2].h= 60;






            SDL_Event e;
            SDL_RendererFlip flip = SDL_FLIP_VERTICAL;
            bool quit = false;
            bool onPlatform = true;
            //size and position
            SDL_Rect position;
            position.y = screen-> h/2 +110;
            position.x = screen->w/2 +120;
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


                for(i=0; i<size1; i++)
                {

                    SDL_RenderCopy(gRenderer, picture[i],NULL,&platforms[i]);
                }

                for(i=0; i<size2; i++)
                {
                    SDL_RenderCopy(gRenderer, caps[i],NULL,&capsRect[i]);
                }


                for(i=0; i<size3; i++)
                {
                    SDL_RenderCopy(gRenderer, bjornDrapare[i],NULL,&bjornDRect[i]);
                }
                for(i=0;i<size2;i++){
                      SDL_RenderCopy(gRenderer, myText[i],NULL,&textRect[i]);
                }

                SDL_RenderCopyEx(gRenderer, player, &spriteClips[frame],&position, 0, NULL, flip);


                SDL_RenderPresent(gRenderer);

            }



        }

    }



    //Destroy window
    SDL_DestroyWindow(gameplay);
    //Quit SDL subsystems


    for(i=0; i<size1; i++)
    {
        SDL_DestroyTexture(picture[i]);
    }

    for(i=0; i<size2; i++)
    {
        SDL_DestroyTexture(caps[i]);
    }

     for(i=0; i<size3; i++)
    {
        SDL_DestroyTexture(bjornDrapare[i]);
    }

     for(i=0; i<size2; i++)
    {
        SDL_DestroyTexture(myText[i]);
    }


    SDL_DestroyTexture(player);
    //  SDL_DestroyWindow(bakgroundTexture);
    SDL_DestroyRenderer(gRenderer);


    SDL_Quit();
    TTF_Quit();

}

