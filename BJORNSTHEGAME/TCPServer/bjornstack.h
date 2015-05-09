/*
Functions acting on the stacks used by the server
Created 2015-04-21 by Jonathan Kåhre
Projekt Grupp 5
*/

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

/* Push the incoming string of a socket to the string-stack */
void pushString(DataStack *stack, char *pushStr);

/* Grab the ID of the socket at the top of the stack */
void popString(DataStack *stack, char* string);

/* Check if the stack is empty */
int isEmptyStrStack(DataStack stack);

/* Check if the stack is full */
int isFullStrStack(DataStack stack);


#endif
