#include "bjornstack.h"

void pushSocketStack(cServ *stack, int pushSock){ // pushes socket id to the stack
	if(isFullStack(*stack)){
		fprintf(stderr, "Error adding to the stack: Stack is full\n");
	}else{
		(*stack).socket[(*stack).population] = pushSock;
		(*stack).population++;
	}
}

int popSocketStack(cServ *stack){ // get a socket id from the stack
	if(isEmptyStack(*stack)){
		fprintf(stderr, "Error taking from stack: Stack is Empty\n");
	}else{
		(*stack).population--;
		return ((*stack).socket[(*stack).population]); 
	}
}

int isEmptyStack(cServ stack){ // check if the stack is empty
	if(stack.population == 0){
		return 1;
	}else
		return 0;
}

int isFullStack(cServ stack){ // check if the stack is full
	if(stack.population > PLAYERCOUNT){
		return 1;
	}else{
		return 0;
	}
}