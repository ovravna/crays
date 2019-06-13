#include "rays.h"
#include <sys/queue.h>
 
uint32_t stage[B_WIDHT * B_HEIGHT];

int create_stage(uint32_t * pixels, uint32_t * stage) {
	/* if (stage != NULL) return 0; */
	
	/* stage = melloc(B_WIDHT * B_HEIGHT * sizeof(uint32_t)); */
	
	for (uint32_t y = 0; y < B_HEIGHT; y++) {
		for (uint32_t x = 0; x < B_WIDHT; x++) {
			if (x == 0 || x == B_WIDHT - 1 || y == 0 || y == B_HEIGHT - 1)
				stage[y * B_WIDHT + x] = 1;
			else if ((y == 4 && x > 0 && x < 6) || (y == 12 && x < B_WIDHT && x > B_WIDHT - 8)) 
				stage[y * B_WIDHT + x] = 1;
			else 
				stage[y * B_WIDHT + x] = 0;
		}
	}
	return 0;
}
void setup(uint32_t * pixels) {
	create_stage(pixels, stage);
}
void draw(uint32_t * pixels) {
/* memset(pixels + HEIGHT / 2, 0x0f0, WIDHT); */
/* for (uint32_t y = 0; y < HEIGHT; y++) { */
	/* 	for (uint32_t x = 0; x < WIDHT; x++) { */
	/* 		uint32_t cx = x - WIDHT/2; */
	/* 		uint32_t cy = y - HEIGHT/2; */
	/* 		double sq = sqrt(cx*cx + cy*cy); */
	/* 		if (sq <= 100 && sq > 98) */
	/* 			pixels[y*WIDHT + x] = 0xff0000; */
	/* 	} */
	/* } */

	stage[B_WIDHT*B_HEIGHT / 2 - (B_WIDHT / 2)] = 2;
	
	draw_stage(pixels, stage);
}

int draw_stage(uint32_t* pixels, uint32_t* stage) {
	/* for (size_t n = 0; n < n_blocks; n++) { */
	/* 	if (blocks[n] != 0) */ 
	/* 		draw_block(pixels, n % B_WIDHT, n / B_HEIGHT); */
	/* } */

	for (uint32_t y = 0; y < B_HEIGHT; y++) {
		for (uint32_t x = 0; x < B_WIDHT; x++) {
			switch (stage[y*B_WIDHT + x]) {
				case 0:
					break;
				case 1:
					draw_block(pixels, x, y); 
					break;
				case 2:
					draw_player(pixels, x, y);
					break;
			}
		}
	}
	return 0;
}
int draw_player(uint32_t* pixels, uint32_t x_block, uint32_t y_block) {

	uint32_t color, edge_color, cx, cy;
	for (uint32_t y = 0; y < BLOCK;y++) {
		for (uint32_t x = 0; x < BLOCK;x++) {
			edge_color = 0xffa500;
			color = 0x005aff;
			cx = x - BLOCK / 2;
			cy = y - BLOCK / 2;
			double r = sqrt(cx*cx + cy*cy);
			if (r < BLOCK/2)
				pixels[(y_block*BLOCK + y)*WIDHT + x_block*BLOCK + x] = edge_color;
			if (r < BLOCK / 2 - 2)
				pixels[(y_block*BLOCK + y)*WIDHT + x_block*BLOCK + x] = color;
		}
	}
	return 0;  
}
int draw_block(uint32_t* pixels, uint32_t x_block, uint32_t y_block) {
	uint32_t color;
	for (uint32_t y = 0; y < BLOCK;y++) {
		for (uint32_t x = 0; x < BLOCK;x++) {
			color = 0xff0000;
			if (y == 0 || y == BLOCK - 1 || x == 0 || x == BLOCK - 0) {
				color = 0x0;	
			}
			
			pixels[(y_block*BLOCK + y)*WIDHT + x_block*BLOCK + x] = color;
		}
	}
	return 0;  
}

