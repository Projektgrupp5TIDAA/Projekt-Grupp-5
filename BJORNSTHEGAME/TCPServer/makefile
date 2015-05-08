SDL2_DIR = /Library/Frameworks/SDL2.framework/Headers
SDL2NET_DIR = /Library/Frameworks/SDL2.framework/Headers
FRAMEWORKS=/Library/Frameworks
WSDL2_INC = C:\MinGW-libs\include
WSDL2_LIB = C:\MinGW-libs\lib

CC = gcc

ifeq ($(OS),Windows_NT)
	CFLAGS=-I$(WSDL2_INC) -L$(WSDL2_LIB) -lmingw32 -lSDL2main -lSDL2 -lSDL2_net
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
