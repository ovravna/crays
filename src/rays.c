#include "rays.h"

typedef struct {
	uint32_t x;
	uint32_t y;

} block;


int draw(uint32_t * pixels) {
	block b = { 2 };
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
	block blocks[B_WIDHT * B_HEIGHT];
	for (uint32_t y = 0; y < B_HEIGHT; y++) {
		for (uint32_t x = 0; x < B_WIDHT; x++) {
			if (x == 0 || x == B_WIDHT - 1 || y == 0 || y == B_HEIGHT - 1)
				draw_block(pixels, x, y);
		}
	}
	return 0;
}

int draw_blocks(uint32_t* pixels, block* blocks, uint32_t n_blocks) {
	for (size_t n = 0; n < n_blocks; n++) {
		draw_block(pixels, blocks[n].x, blocks[n].y);
	}
	return 0;
}

int draw_block(uint32_t* pixels, uint32_t x_block, uint32_t y_block) {
		
	for (uint32_t y = 0; y < BLOCK;y++) {
		for (uint32_t x = 0; x < BLOCK;x++) {
			uint32_t color = 0xff0000;
			if (y == 0 || y == BLOCK - 1 || x == 0 || x == BLOCK - 0) {
				color = 0x0;	
			}
			
			pixels[(y_block*BLOCK + y)*WIDHT + x_block*BLOCK + x] = color;
		}
	}
	return 0;  
}

