#include "bjornshared.h"

int is_set(int comp, int bit){
	return comp & 1<<bit;
}

void set_bit(int *num, int bit){
	*num = *num | 1<<bit;
}

void parseChat(char* inc, int hops, int len){
	int i;
	for(i=0;i<len;i++){
		*(inc+i) = *(inc+i+hops);
	}
}