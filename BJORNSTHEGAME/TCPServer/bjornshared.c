#include "bjornshared.h"

/* Check if the bit at place 'bit' is set in int comp */
int is_set(int comp, int bit){
	return comp & 1<<bit;
}

/* Set the bit at place 'bit' of num to 1 */
void set_bit(int *num, int bit){
	*num = *num | 1<<bit;
}

/* Parses the string 'hops' hops forward */
void parseChat(char* inc, int hops, int len){
	int i;
	for(i=0;i<len;i++){
		*(inc+i) = *(inc+i+hops);
	}
}