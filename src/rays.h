#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
/* #include <gmodule.h> */

#define BLOCK 20
#define B_WIDHT 16
#define B_HEIGHT 16
#define WIDHT BLOCK * B_WIDHT
#define HEIGHT BLOCK * B_HEIGHT
#define SCREEN_SIZE WIDHT*HEIGHT

typedef struct {
	uint32_t x;
	uint32_t y;

} block;

void setup(uint32_t * pixels);
void draw(uint32_t * pixels);
int draw_block(uint32_t * pixels, uint32_t x_block, uint32_t y_block);
int draw_stage(uint32_t* pixels, uint32_t* stage);
int draw_player(uint32_t* pixels, uint32_t x_block, uint32_t y_block);
int create_stage(uint32_t * pixels, uint32_t * stage);
