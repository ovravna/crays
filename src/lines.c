#include "lines.h"
#include <math.h>
#include <stdlib.h>

void draw_line_low(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);
void draw_line_high(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color);

void draw_line(uint32_t * pixels, int x0, int y0, int x1, int y1, uint32_t color) {
	if (abs(y1 - y0) < abs(x1 - x0)) {
		if (x0 > x1)
			draw_line_low(pixels, x1, y1, x0, y0, color);
		else
			draw_line_low(pixels, x0, y0, x1, y1, color);
	} 
	else { 
		if (y0 > y1)
			draw_line_high(pixels, x1, y1, x0, y0, color);
		else 
			draw_line_high(pixels, x0, y0, x1, y1, color);
	}
}


void draw_line_low(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color){
	int dx = x1 - x0;
	int dy = y1 - y0; 
	int yi = y0;
	if (dy < 0) {
		yi = -1;
		dy = -dy;
	}
	int D = 2*dy - dx;
	uint32_t y = y0;
	for (uint32_t x = x0; x < x1; x++) {
		pixels[y * WIDHT + x] = color;
		if (D > 0) {
			y += yi; 
			D -= 2*dx;
		}
		D += 2*dy;
	}
}
void draw_line_high(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t color) {

	int dx = x1 - x0;
	int dy = y1 - y0; 
	int xi = 1;
	if (dx < 0) {
		xi = -1;
		dx = -dx;
	}
	int D = 2*dx - dy;
	uint32_t x = x0;
	for (uint32_t y = y0; y < y1; y++) {
		pixels[y * WIDHT + x] = color;
		if (D > 0) {
			x += xi; 
			D -= 2*dy;
		}
		D += 2*dx;
	}
}

