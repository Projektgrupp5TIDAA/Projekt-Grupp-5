/*
Functions used for bit-manipulation on TCP-packages
Created 2015-04-22 by Jonathan Kåhre
Projekt Grupp 5
*/

#ifndef BJORN_SHARED_H
#define BJORN_SHARED_H
#include <stdio.h>

int is_set(int comp, int bit);

void set_bit(int *num, int bit);

void parseChat(char* inc, int hops, int len);

#endif
