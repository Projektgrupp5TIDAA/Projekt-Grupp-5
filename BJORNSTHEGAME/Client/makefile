SDL2_DIR = /Library/Frameworks/SDL2.framework/Headers
SDL2NET_DIR = /Library/Frameworks/SDL2.framework/Headers
FRAMEWORKS=/Library/Frameworks

CC = gcc

ifeq ($(OS),Windows_NT)
	CFLAGS=-g -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	objectlist = bjornsclient.o bjornstartup.o lobby.o gameplay.o clientthreads.o animation.o
else
	UNAME_TARGET := $(shell uname -s)
	ifeq ($(UNAME_TARGET),Linux)
		CFLAGS=-g -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	objectlist = bjornsclient.o bjornstartup.o lobby.o gameplay.o clientthreads.o animation.o
	endif
	ifeq ($(UNAME_TARGET),Darwin)
		CFLAGS=-I$(SDL2_DIR) -I$(SDL2NET_DIR) -F$(FRAMEWORKS) -framework SDL2 -framework SDL2_net -framework SDL2_image  -framework SDL2_mixer  -framework SDL2_ttf
	objectlist = bjornsclient.c bjornstartup.c lobby.c gameplay.c clientthreads.c animation.c
	endif
endif

bjornserver : $(objectlist)
	$(CC) $(objectlist) $(CFLAGS) -std=gnu99  -o bjornsclient.out
