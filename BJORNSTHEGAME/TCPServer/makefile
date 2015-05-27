SDL2_DIR = /Library/Frameworks/SDL2.framework/Headers
SDL2NET_DIR = /Library/Frameworks/SDL2.framework/Headers
FRAMEWORKS=/Library/Frameworks

CC = gcc

ifeq ($(OS),Windows_NT)
	CFLAGS=-Wall -g -lmingw32 -lSDL2main -lSDL2 -lSDL2_net
	objectlist = bjornserver.o bjornthreads.o bjornstack.o bjornshared.o
else
	UNAME_TARGET := $(shell uname -s)
	ifeq ($(UNAME_TARGET),Linux)
		CFLAGS=-lSDL2 -lSDL2_net
		objectlist = bjornserver.o bjornthreads.o bjornstack.o bjornshared.o
	endif
	ifeq ($(UNAME_TARGET),Darwin)
		CFLAGS=-I$(SDL2_DIR) -I$(SDL2NET_DIR) -F$(FRAMEWORKS) -framework SDL2 -framework SDL2_net
		objectlist = bjornserver.c bjornthreads.c bjornstack.c bjornshared.c
	endif
endif

bjornserver : $(objectlist)
	$(CC) $(objectlist) $(CFLAGS) -o bjornserver.out
