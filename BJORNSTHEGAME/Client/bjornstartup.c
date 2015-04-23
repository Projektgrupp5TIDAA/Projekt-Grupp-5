#include "bjornstartup.h"

int menu(SDL_Window* window, StartInf startup){
  	SDL_Surface *screen = SDL_GetWindowSurface(window);
	int quit = 0, mouse[2] = {0};
    int resize_timer=0;

	/* Load image-surfaces */
	SDL_Surface* background = IMG_Load("../Images/menu/MenuBack.png");
  	SDL_Surface* playbutton = IMG_Load("../Images/menu/MenuPlayButtonMin.png");
  	SDL_Surface* tapir = IMG_Load("../Images/menu/tapir_image.png");
  	SDL_Surface* exitbutton = IMG_Load("../Images/menu/door.png");

  	/* Load colour, font and then render text-surfaces */
	SDL_Colour black={0,0,0};
  	TTF_Font *font = TTF_OpenFont("../Images/menu/StencilStd.ttf", 120);
  	SDL_Surface *title = TTF_RenderText_Solid(font, "MENU", black);

  	/* Load music, set volume and start */
	Mix_Music *music = Mix_LoadMUS("../Music/Mechanolith.mp3");
	Mix_Chunk *uselt = Mix_LoadWAV("../Sounds/uselt.wav");
	Mix_VolumeMusic(64);
	Mix_PlayMusic(music, -1);

	/* Initialize rectangles */
	SDL_Rect titleplacement = {(screen->w/2 - ((title->w)/2)), 20, 0, 0};
	SDL_Rect buttonplacement = {((screen->w/2) - 125), (screen->h/2), 250, 60};
	SDL_Rect button2placement = {((screen->w/2) - 125), (screen->h/2 + 70), 250, 60};
	SDL_Rect button3placement = {((screen->w/2) - 125), (screen->h/2 - 70), 250, 60};
	SDL_Rect exitplacement = {(screen->w - 126), (screen->h - 206), 120, 200};
	SDL_Rect tapirplacement = {0, (screen->h - 66), 100, 66};

	while(!quit){
		SDL_PumpEvents();
		SDL_GetMouseState(&mouse[0], &mouse[1]);

		if(getMouseBounds(mouse, exitplacement)){
            if(SDL_GetMouseState(NULL,NULL)& SDL_BUTTON(SDL_BUTTON_LEFT)){
			Mix_PlayChannel(-1, uselt, 1);
			SDL_Delay(4000);
			SDLNet_TCP_Send(*(startup.socket), "EXITCONNECTION", 14);

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
		}else

		if(getMouseBounds(mouse, tapirplacement)){
			//get name, then ip then connect
			printf("TAPIR\n");
			getName(startup.playerName, 20, screen); // get name through the readkeyboard function
			getIP(startup.targethost, screen); // get the host address and port connection
			*(startup.socket) = SDLNet_TCP_Open(startup.targethost); // open socket with the targethost
			SDLNet_TCP_Send(*(startup.socket), (void*)startup.playerName, 20); //socket, data, lenght
			connectToHost(startup.targethost, startup.socket);
		}
        else if(getMouseBounds(mouse, tapirplacement)){ // resize tapir, should be moved to the function above?
            SDL_Surface* helpbutton = IMG_Load("../Images/menu/door.png"); // if the mouse still on SDL_Delay
                                                                            // change later
            SDL_Rect tapirplacement = {0, (screen->h - 2000), 700, 2000};
            SDL_BlitScaled(tapir, NULL, screen, &tapirplacement);
            if(resize_timer==0){
                SDL_UpdateWindowSurface(window);
                SDL_Delay(3000);
                resize_timer=1;
            }
        }
            resize_timer=0;
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
}

    int getMouseBounds(int mouse[2], SDL_Rect rect){
	if(mouse[0]>rect.x && mouse[0]<(rect.x+rect.w)){
		if(mouse[1]>rect.y && mouse[1]<(rect.y+rect.h)){
			return 1;
		}
	}
	return 0;
}

int getName(char* name, int len, SDL_Surface* screen){
	//get name return success or failure
	readKeyboard(name, len);
	return 0;
}

int getIP(IPaddress* targethost, SDL_Surface* screen){ // get the adress/target host and port through the readkeyboard function
	char address[15] = {0};
	char port[5] = {0};
	printf("Read name, time to read address!\n");
	readKeyboard(address, 9);
	printf("Read address: %s, time to read port!\n", address);
	readKeyboard(port, 4);
	printf("Read port: %s, time to resolve the host!\n", port);

	if (SDLNet_ResolveHost(targethost, address, atoi(port)) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", address, port, SDLNet_GetError());
		return 1;
	}else
		return 0;
}

int connectToHost(IPaddress* targethost, TCPsocket* socket){
	//try to connect to host return success or failure
}

int readKeyboard(char* output, int len){
	SDL_Event event;
	SDL_StartTextInput();
	while(len != 0){
		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_TEXTINPUT){
				strcat(output, event.text.text);
				printf("%s\n", output);
				len--;
			}
			if(event.type == SDL_KEYDOWN){
				if(event.key.keysym.sym == SDLK_RETURN){
					len = 0;
				}
			}
		}
	}
	SDL_StopTextInput();
}