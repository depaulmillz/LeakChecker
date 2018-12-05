#pragma once
#include <stdlib.h>

void printMem();
void track(size_t size, void *ptr, char b, char p);
void *malloc(size_t);
void free(void *);
void track(size_t, void *, char, char);