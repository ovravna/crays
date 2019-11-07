#ifndef INPUTS_H
#define INPUTS_H

#include <string.h>


enum arrow { NoPress, Up, Right, Down, Left };
#define N 5
/* int pressed[N]; */
int current_key;

void keypress(int key);

void reset_keypress();

int get_keypress();

#endif
