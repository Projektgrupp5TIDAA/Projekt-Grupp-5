#ifndef BJORN_STACK_H
#define BJORN_STACK_H
#include "bjornthreads.h"

void pushSocketStack(cServ *stack, int pushSock);

int popSocketStack(cServ *stack);

int isEmptyStack(cServ stack);

int isFullStack(cServ stack);

#endif