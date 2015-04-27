SDL2_DIR = /Library/Frameworks/SDL2.framework/Headers
SDL2NET_DIR = /Library/Frameworks/SDL2.framework/Headers
FRAMEWORKS=/Library/Frameworks
WSDL2_INC = C:\MinGW-libs\include
WSDL2_LIB = C:\MinGW-libs\lib

CC = gcc

ifeq ($(OS),Windows_NT)
	CFLAGS=-I$(WSDL2_INC) -L$(WSDL2_LIB) -lmingw32 -lSDL2main -lSDL2 -lSDL2_net
	objectlist = bjornsclient.o bjornstartup.o
else
	UNAME_TARGET := $(shell uname -s)
	ifeq ($(UNAME_TARGET),Linux)
		CFLAGS=-lSDL2 -lSDL2_net -lSDL2_image -lSDL2_mixer -lSDL2_ttf
		objectlist = bjornsclient.o bjornstartup.o
	endif
	ifeq ($(UNAME_TARGET),Darwin)
		CFLAGS=-I$(SDL2_DIR) -I$(SDL2NET_DIR) -F$(FRAMEWORKS) -framework SDL2 -framework SDL2_net -framework SDL2_image  -framework SDL2_mixer  -framework SDL2_ttf
		objectlist = bjornsclient.c bjornstartup.c
	endif
endif

bjornserver : $(objectlist)
	$(CC) -o bjornsclient3.out $(CFLAGS) $(objectlist)
