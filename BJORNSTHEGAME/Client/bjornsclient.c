#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2_ttf/SDL_ttf.h>


int main(int argc, char const *argv[])
{
    SDL_Window* window;
    SDL_INIT_VIDEO;
    window = SDL_CreateWindow("HEJ",						//Window name
                              SDL_WINDOWPOS_UNDEFINED,		//x-position
                              SDL_WINDOWPOS_UNDEFINED,		//y-position
                              720,
                              400,
                              SDL_WINDOW_FULLSCREEN_DESKTOP	//flags etc
                              );
    
    SDL_Renderer *test = SDL_CreateRenderer(window, -1, 0);
    
    if (window==NULL)
    {
        printf("Couldnt create window\n");
    }
    
    TTF_Init();
    TTF_Font *font;
    font = TTF_OpenFont("Metropolitan-Text.ttf", 16);
    SDL_Color color={0,0,0};
    SDL_Surface *text_surface;
    SDL_Texture* testmessage = SDL_CreateTextureFromSurface(test, text_surface);
    SDL_Rect testrect;
    testrect.x = 0;
    testrect.y = 0;
    testrect.w = 100;
    testrect.h = 100;
    if(!(text_surface=TTF_RenderText_Solid(font, "Hello!", color))){
        printf("FEEEL\n");
    } else {
        SDL_RenderCopy(test, testmessage, NULL, &testrect);
    }
    
    
    SDL_Delay(8000); //8 sek
    
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
    
}
