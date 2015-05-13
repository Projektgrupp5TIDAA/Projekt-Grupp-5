/*
Functions acting on the stacks used by the server
Created 2015-04-21 by Jonathan Kåhre
Projekt Grupp 5
*/

#include <stdio.h>
#include <string.h>
#include "bjornstack.h"
#include "bjornshared.h"

/* Push the struct to the top of the stack */
void pushStack(ThreadStack *stack, tinfo *pushThread){
	if(isFullStack(*stack)){
		fprintf(stderr, "Error adding to the stack: Stack is full\n");
	}else{
		(*stack).thread[(*stack).population] = pushThread;
		(*stack).population++;
	}
}

/* Grab the struct at the top of the stack */
tinfo* popStack(ThreadStack *stack){
	if(isEmptyStack(*stack)){
		fprintf(stderr, "Error taking from stack: Stack is Empty\n");
		return NULL;
	}else{
		(*stack).population--;
		return ((*stack).thread[(*stack).population]);
	}
}

/* Check if the stack is empty */
int isEmptyStack(ThreadStack stack){
	if(stack.population == 0){
		return 1;
	}else
		return 0;
}

/* Check if the stack is full */
int isFullStack(ThreadStack stack){
	if(stack.population > PLAYERCOUNT-1){
		return 1;
	}else{
		return 0;
	}
}

/* Push the incoming string onto the stack */
void pushString(DataStack *stack, char* pushStr, int len){
	if(isFullStrStack(*stack)){
		fprintf(stderr, "Error adding to the stack: Stack is full\n");
	}else{
		printf("PUSHING TO THE STACK\n");
		memcpy(stack->item[stack->population], pushStr, len);
		//strcpy(stack->item[stack->population], pushStr);
		(stack->population)++;
		printf("PUSHING TO THE STACK END\n");
	}
}

/* Grab the string at the top of the stack */
void popString(DataStack *stack, char* string, int len){
	if(isEmptyStrStack(*stack)){
		fprintf(stderr, "Error taking from stack: Stack is Empty\n");
	}else{
		printf("TAKING FROM THE STACK\n");
		(stack->population)--;
		emptyString(string, strlen(string));
		memcpy(string, stack->item[stack->population], len);
		printf("TAKING FROM THE STACK END\n");
		//strcpy(string, *(stack->item));
	}
}

/* Check if the stack is empty */
int isEmptyStrStack(DataStack stack){
	if(stack.population == 0){
		return 1;
	}else
		return 0;
}

/* Check if the stack is full */
int isFullStrStack(DataStack stack){
	if(stack.population > PLAYERCOUNT-1){
		return 1;
	}else{
		return 0;
	}
}
