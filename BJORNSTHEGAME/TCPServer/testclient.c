#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SDL2_net/SDL_net.h"

int main(int argc, char **argv)
{
	TCPsocket sd;
	IPaddress srvadd;
	int quit;
	char outgoing[512] = {0};

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s host port\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	/* Initialize SDL_net */
	if (SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Resolve server name  */
	if (SDLNet_ResolveHost(&srvadd, argv[1], atoi(argv[2])) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", argv[1], atoi(argv[2]), SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Open a socket on random port */
	if (!(sd = SDLNet_TCP_Open(&srvadd)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}

	/* Main loop */
	quit = 0;
	while (!quit)
	{
		printf("Input: ");
		scanf("%s", outgoing);

		SDLNet_TCP_Send(sd, (void*)outgoing, 512); /* This sets the p->channel */
	}

	SDLNet_TCP_Close(sd);
	SDLNet_Quit();

	return EXIT_SUCCESS;
}
