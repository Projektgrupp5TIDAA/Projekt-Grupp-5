#ifndef BJORN_STACK_H
#define BJORN_STACK_H
#include "bjornthreads.h"

/* Push the incoming tinfo to the stack */
void pushStack(ThreadStack *stack, tinfo *pushSock);

/* Grab the ID of the socket at the top of the stack */
tinfo* popStack(ThreadStack *stack);

/* Check if the stack is empty */
int isEmptyStack(ThreadStack stack);

/* Check if the stack is full */
int isFullStack(ThreadStack stack);

#endif
