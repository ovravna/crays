#ifndef RAYS_H
#define RAYS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "values.h"

typedef struct {
	uint32_t x, y;

} block_t;

typedef struct {
	uint32_t x, y; // Position in pixels
	double a; // View angel
} player_t;

typedef struct s {
	uint32_t x0, y0, x1, y1;
} ray;

void setup(uint32_t * pixels);
void loop(uint32_t * pixels);

int draw_block(uint32_t * pixels, uint32_t x_block, uint32_t y_block, uint32_t color, uint32_t bg_color);
int draw_stage(uint32_t* pixels, uint32_t* stage);
int draw_player(uint32_t * pixels, player_t player); 
int create_stage(uint32_t * pixels, uint32_t * stage);
int cast_rays(uint32_t * pixels, player_t player, uint32_t * stage, ray** rays, uint32_t n_lines, double fov);
#endif
