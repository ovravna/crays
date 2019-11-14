#ifndef INPUTS_H
#define INPUTS_H

#include <string.h>
#include <stdint.h>

enum arrow { 
    NoPress = 0,
    Up = 1, Right = 2, Down = 4, Left = 8 };
#define N 5
/* int pressed[N]; */
int current_key;

void keypress(uint32_t key);

void keyup(uint32_t key);

void reset_keypress();

int get_keypress();

#endif
