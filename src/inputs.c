#include "inputs.h"

void keypress(int key) {
   current_key = key; 
}

void reset_keypress() {
    current_key = 0;
}

int get_keypress() {
    int res = current_key;
    reset_keypress();
    return res;
}


