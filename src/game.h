#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "values.h" 

void setup(uint32_t * pixels);
void loop(uint32_t * pixels);

void keysDown(int key);

typedef struct {
	double x, y;
} vec;

typedef struct {
	int x, y;
} ivec;

#endif
