#include "inputs.h"

unsigned int keypress_mask = 0;

void keypress(uint32_t key) {
   keypress_mask |= key;
   current_key = key; 
}

void reset_keypress() {
    keypress_mask = 0;
    current_key = 0;
}

void keyup(uint32_t key) {

    keypress_mask &= ~key;

}

int get_keypress() {
    int res = keypress_mask;
    /* reset_keypress(); */
    return res;
}


