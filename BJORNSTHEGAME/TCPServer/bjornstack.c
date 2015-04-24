#include "bjornstack.h"

/* Push the ID of a socket to the cServ stack top */
void pushSocketStack(cServ *stack, int pushSock){
	if(isFullStack(*stack)){
		fprintf(stderr, "Error adding to the stack: Stack is full\n");
	}else{
		(*stack).socket[(*stack).population] = pushSock;
		(*stack).population++;
	}
}

/* Grab the ID of the socket at the top of the stack */
int popSocketStack(cServ *stack){
	if(isEmptyStack(*stack)){
		fprintf(stderr, "Error taking from stack: Stack is Empty\n");
	}else{
		(*stack).population--;
		return ((*stack).socket[(*stack).population]); 
	}
}

/* Check if the stack is empty */
int isEmptyStack(cServ stack){
	if(stack.population == 0){
		return 1;
	}else
		return 0;
}

/* Check if the stack is full */
int isFullStack(cServ stack){
	if(stack.population > PLAYERCOUNT){
		return 1;
	}else{
		return 0;
	}
}