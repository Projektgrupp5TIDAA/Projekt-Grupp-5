#include "bjornstartup.h"
#include "lobby.h"

int menu(StartInfo startup){
    int quit = 0, mouse[2] = {0};
    char packet[PACKETSIZE];

    /* Create window and get the surface */
    SDL_Window* window = SDL_CreateWindow("BJORNS THE GAME - MENU", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    if (window==NULL){
        printf("Window could not be created.\n");
    }
    SDL_Surface *screen = SDL_GetWindowSurface(window);

    /* Load image-surfaces */
    SDL_Surface* background = IMG_Load("../Images/menu/MenuBack.png");
    if(background == NULL){
        printf("BG NULL\n");
    }
    SDL_Surface* playbutton = IMG_Load("../Images/menu/MenuPlayButtonMin.png");
    SDL_Surface* tapir = IMG_Load("../Images/menu/tapir_image.png");
    SDL_Surface* exitbutton = IMG_Load("../Images/menu/door.png");
    SDL_Surface* yesbutton = IMG_Load("../Images/menu/YesButton.png");
    SDL_Surface* nobutton = IMG_Load("../Images/menu/NoButton.png");

    /* Load colour, font and then render text-surfaces */
    SDL_Colour black={0,0,0};
    SDL_Colour white={255,255,255};
    TTF_Font *font = TTF_OpenFont("../Images/menu/StencilStd.ttf", 120);
    TTF_Font *fontsmall = TTF_OpenFont("../Images/menu/coolvetica.ttf", 30);
    SDL_Surface *title = TTF_RenderText_Solid(font, "MENU", black);

    /* Load music, set volume and start */
    Mix_Music *music = Mix_LoadMUS("../Sounds/Music/Mechanolith.mp3");
    Mix_Chunk *uselt = Mix_LoadWAV("../Sounds/uselt.wav");
    Mix_Chunk *tmp;
    Mix_VolumeMusic(64);
    Mix_PlayMusic(music, -1);

    /* Initialize rectangles */
    SDL_Rect titleplacement = {(screen->w/2 - ((title->w)/2)), 20, 0, 0};
    SDL_Rect buttonplacement = {((screen->w/2) - 125), (screen->h/2), 250, 60};
    SDL_Rect button2placement = {((screen->w/2) - 125), (screen->h/2 + 70), 250, 60};
    SDL_Rect button3placement = {((screen->w/2) - 125), (screen->h/2 - 70), 250, 60};
    SDL_Rect exitplacement = {(screen->w - 126), (screen->h - 206), 120, 200};
    SDL_Rect tapirplacement = {0, (screen->h - 66), 100, 66};
    SDL_Rect menutextpos = {230, 150, 0, 0};
    SDL_Rect yespos = {200, 400, 180, 64};
    SDL_Rect nopos = {420, 400, 180, 64};

    while(!quit){
        SDL_PumpEvents();
        SDL_GetMouseState(&mouse[0], &mouse[1]);

        if(getMouseBounds(mouse, exitplacement)){
            if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
                Mix_PlayChannel(-1, uselt, 1);
                SDL_Delay(4000);

                /* Free the used resources and return*/
                SDL_FreeSurface(title);
                SDL_FreeSurface(exitbutton);
                SDL_FreeSurface(playbutton);
                SDL_FreeSurface(tapir);
                Mix_FreeMusic(music);
                Mix_FreeChunk(uselt);
                TTF_CloseFont(font);
                return 1;
            }
        }else if(getMouseBounds(mouse, tapirplacement)){ //get name, then ip then connect
            if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
                getName(startup.playerName, 20, window); // get name through the readkeyboard function
                if((getIP(startup.targethost, window))){ // get the host address and port connection
                    fprintf(stderr, "Could not resolve hostname.\n");
                    return 1;
                }else{
                    *(startup.socket) = SDLNet_TCP_Open(startup.targethost); // open socket with the targethost
                    if(*(startup.socket) != NULL){
                        if(!(SDLNet_TCP_Send(*(startup.socket), "I", 1))){ //socket, data, length
                            printf("Could not connect to host: %s\n", SDLNet_GetError());
                        }else{
                            while(1){
                                if(SDLNet_TCP_Recv(*(startup.socket), packet, PACKETSIZE)){
                                    printf("INFO RECIEVED: %s\n", packet);
                                    SDL_Surface* serverscreen = IMG_Load("../Images/menu/ConnectConfirmScreen.png");
                                    SDL_BlitSurface(serverscreen, NULL, screen, NULL);
                                    textToScreen(fontsmall, menutextpos, window, packet);
                                    SDL_BlitScaled(yesbutton, NULL, screen, &yespos);
                                    SDL_BlitScaled(nobutton, NULL, screen, &nopos);
                                    SDL_UpdateWindowSurface(window);
                                    break;
                                }else SDL_Delay(10);
                            }
                            while(1){//if yes-button open TCP connection with targethost
                                SDL_PumpEvents();
                                SDL_GetMouseState(&mouse[0], &mouse[1]);
                                if(getMouseBounds(mouse, yespos)){
                                    if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
                                        *(startup.socket) = SDLNet_TCP_Open(startup.targethost);
                                        if(*(startup.socket) == NULL) return 1;
                                        SDLNet_TCP_Send(*(startup.socket), "C", 1);
                                        SDL_Delay(100);
                                        SDLNet_TCP_Send(*(startup.socket), startup.playerName, 20);
                                        SDL_DestroyWindow(window); // close when done and goto lobby
                                        return 0;
                                    }
                                }else if(getMouseBounds(mouse, nopos)){
                                    if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else if(getMouseBounds(mouse, button3placement)){
            if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
                //Mix_PlayChannel(-1, gifwetsvisfel, 1);

                tmp=randomMusic();
                Mix_PlayChannel(-1, tmp, 1);
                SDL_Delay(100);
            }
        }
        else if(getMouseBounds(mouse, button2placement)){
            if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
                //Mix_PlayChannel(-1 ,sasvart, 1);
                tmp=randomMusic();
                Mix_PlayChannel(-1, tmp, 1);
                SDL_Delay(100);
            }
        }
        else if(getMouseBounds(mouse, buttonplacement)){
            if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
                //Mix_PlayChannel(-1, tasantid, 1);
                tmp=randomMusic();
                Mix_PlayChannel(-1, tmp, 1);
                SDL_Delay(100);
            }
        }

        /* Blit the images to the screen */
        SDL_BlitScaled(background, NULL, screen, NULL);
        SDL_BlitSurface(title, NULL, screen, &titleplacement);
        SDL_BlitScaled(playbutton, NULL, screen, &buttonplacement);
        SDL_BlitScaled(playbutton, NULL, screen, &button2placement);
        SDL_BlitScaled(playbutton, NULL, screen, &button3placement);
        SDL_BlitScaled(exitbutton, NULL, screen, &exitplacement);
        SDL_BlitScaled(tapir, NULL, screen, &tapirplacement);

        /* Update the window */
        SDL_UpdateWindowSurface(window);
    }
    return 0;
}

/* Checks if the incoming coordinates are within the rectangle bounds */
int getMouseBounds(int mouse[2], SDL_Rect rect){
    if(mouse[0]>rect.x && mouse[0]<(rect.x+rect.w)){
        if(mouse[1]>rect.y && mouse[1]<(rect.y+rect.h)){
            return 1;
        }
    }
    return 0;
}

/* Saves the name in the incoming variable aswell as posting it to the window */
int getName(char* name, int len, SDL_Window* window){
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    SDL_Surface *namemenu = IMG_Load("../Images/menu/MenuNameScreen.png");
    SDL_BlitScaled(namemenu, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);
    readKeyboardToMenuWindow(name, len, window, namemenu);
    return 0;
}

/* Saves the IP-address into the incoming variable aswell as posting it to the screen */
int getIP(IPaddress* targethost, SDL_Window* window){
    char address[15] = {0};
    char port[5] = {0};
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    SDL_Surface *IPmenu = IMG_Load("../Images/menu/MenuIPScreen.png");
    SDL_Surface *portmenu = IMG_Load("../Images/menu/MenuPortScreen.png");

    SDL_BlitScaled(IPmenu, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);
    printf("Read name, time to read address!\n");
    readKeyboardToMenuWindow(address, 15, window, IPmenu);
    SDL_BlitScaled(portmenu, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);
    printf("Read address: %s, time to read port!\n", address);
    readKeyboardToMenuWindow(port, 5, window, portmenu);
    printf("Read port: %s, time to resolve the host!\n", port);

    if (SDLNet_ResolveHost(targethost, address, atoi(port)) == -1){
        fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", address, atoi(port), SDLNet_GetError());
        return 1;
    }else
        return 0;
}

/* Posts the incoming string to the window, with the incoming font aswell as the incoming placement */
int textToScreen(TTF_Font *font, SDL_Rect place, SDL_Window* window, char* text){
    SDL_Surface *screen = SDL_GetWindowSurface(window);
    SDL_Colour black={0,0,0};
    SDL_Surface *textsurface = TTF_RenderText_Solid(font, text, black);
    SDL_BlitSurface(textsurface, NULL, screen, &place);
    SDL_UpdateWindowSurface(window);
    return 0;
}

/* reads the keyboard input into the string with the maximum length */
int readKeyboard(char* output, int len){
    char temp[len];
    emptyString(temp, len);
    int initlen = len;
    SDL_Event event;
    SDL_StartTextInput();
    while(len > 0){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_TEXTINPUT){
                temp[initlen-len] = *(event.text.text);
                printf("LEN: %d, STRING: %s\n", len, temp);
                len--;
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_RETURN){
                    strcpy(output, temp);
                    len = 0;
                }
            }
        }
    }
    SDL_StopTextInput();
    return 0;
}

/* Reads keyboard input while it post the input to the screen */
int readKeyboardToMenuWindow(char* output, int len, SDL_Window* window, SDL_Surface* bg){
    TTF_Font *font = TTF_OpenFont("../Images/menu/coolvetica.ttf", 40);
    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_Rect place = {230,142, 0,0};
    char temp[len];
    emptyString(temp, len);
    int initlen = len;
    SDL_Event event;
    SDL_StartTextInput();
    while(len > 0){
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_TEXTINPUT){
                temp[initlen-len] = *(event.text.text);
                printf("LEN: %d, STRING: %s\n", len, temp);
                textToScreen(font, place, window, temp); // write text on the screen
                len--;
            }
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_RETURN){
                    strcpy(output, temp);
                    len = 0;
                }
                if(event.key.keysym.sym == SDLK_BACKSPACE){ // erase text
                    if(len<initlen){
                        len++;
                        temp[initlen-len]=0;
                        SDL_BlitScaled(bg, NULL, screen, NULL);
                        SDL_UpdateWindowSurface(window);
                        textToScreen(font, place, window, temp);
                        printf("Backspace: %s\n", temp);
                    }else printf("No text to delete.\n");
                }
            }
        }
    }
    SDL_StopTextInput();
    return 0;
}

/* This function empties a string from a pointer with the length 'len' */
int emptyString(char* incoming, int len){
    int i;
    for(i=0; i<len; i++){
        *(incoming+i) = 0;
    }
    return 0;
}

Mix_Chunk* randomMusic()
{
    int i;
    Mix_Chunk *gifwetsvisfel =Mix_LoadWAV("../Sounds/gifwetsvisfel.wav");
    Mix_Chunk *sasvart= Mix_LoadWAV("../Sounds/sasvart.wav");
    Mix_Chunk *tasantid= Mix_LoadWAV("../Sounds/tasantid.wav");
    Mix_Chunk *temp;
    for(i=0; i<3; i++)
    {

        int music= rand()%2+0;

        if(music ==0)
        {
            temp=gifwetsvisfel;
            SDL_Delay(100);
            break;
        }
        if(music==1)
        {
            temp=sasvart;
            SDL_Delay(100);
            break;
        }
        if(music==2)
        {
            temp=tasantid;
            SDL_Delay(100);
            break
        }


    }
    return temp;

}

