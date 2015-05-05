#include <stdio.h>
#include <string.h>
#include "bjornstack.h"

/* Push the ID of a socket to the cServ stack top */
void pushStack(ThreadStack *stack, tinfo *pushThread){
	if(isFullStack(*stack)){
		fprintf(stderr, "Error adding to the stack: Stack is full\n");
	}else{
		(*stack).thread[(*stack).population] = pushThread;
		(*stack).population++;
	}
}

/* Grab the ID of the socket at the top of the stack */
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

/* Push the incoming string of a socket to the string-stack */
void pushString(DataStack *stack, char *pushStr){
	if(isFullStrStack(*stack)){
		fprintf(stderr, "Error adding to the stack: Stack is full\n");
	}else{
		stack->item[stack->population] = *pushStr;
		(stack->population)++;
	}
}

/* Grab the ID of the socket at the top of the stack */
char* popString(DataStack *stack){
	if(isEmptyStrStack(*stack)){
		fprintf(stderr, "Error taking from stack: Stack is Empty\n");
		return NULL;
	}else{
		(stack->population)--;
		return (stack->item);
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
