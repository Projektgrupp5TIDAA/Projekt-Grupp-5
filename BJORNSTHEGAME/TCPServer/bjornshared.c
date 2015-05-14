/*
Functions used for bit-manipulation on TCP-packages
Created 2015-04-22 by Jonathan Kåhre
Projekt Grupp 5
*/

#include "bjornshared.h"

/* Check if the bit at place 'bit' is set in int 'comp' */
int is_set(int comp, int bit){
	return (comp & 1<<bit);
}

/* Set the bit at place 'bit' of 'num' to 1 */
void set_bit(int *num, int bit){
	*num = *num | 1<<bit;
}

/* Parses the string 'hops' places forward */
void parseString(char* inc, int hops, int len){
    int i;
    if(hops>0){
        for(i=0;i<len;i++){
            *(inc+i) = *(inc+i+hops);
        }
    }else if(hops<0){
        for(i=(len-1);i>0;i--){
            *(inc+i) = *(inc+i+hops);
        }
    }
}

/* This function empties a string from a pointer with the length 'len' */
int emptyString(char* incoming, int len){
    int i;
    for(i=0; i<len; i++){
        *(incoming+i) = 0;
    }
    return 0;
}
