#ifndef KEYEVENT_H_INCLUDED
#define KEYEVENT_H_INCLUDED

void keyEvent()
{
    SDL_Event event;
    bool gameRunning = true;

    while (gameRunning)
    {
        if(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
            {
                gameRunning = false;
            }
            if(event.type == SDL_KEYDOWN)
            {
                SDLKey keyPressed = event.key.keysym.sym;

                switch (keyPressed)
                {
                    //SDLK_ And the button for the event
                    case SDLK_ESCAPE:
                        gameRunning =false;
                        break;
                    case SDLK_UP:
                        y--;
                        break;
                    case SDLK_DOWN:
                        y++;
                        break;
                    case SDLK_LEFT:
                        x--;
                        break;
                    case SDLK_RIGHT:
                        x++;
                        break;
                }
            }
        }
    }
}

#endif // KEYEVENT_H_INCLUDED
