#include "lines.h"
#include "stdlib.h"
#include "math.h"


//DDA from https://youtu.be/W5P8GlaEOSI
void draw_line(uint32_t * pixels, int x0, int y0, int x1, int y1, uint32_t color) {
	
    int dx = x1 - x0;
    int dy = y1 - y0;

    uint32_t steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    
    double x = x0,
	   y = y0;
    
    double xinc = (double)(dx) / steps;
    double yinc = (double)(dy) / steps;

    for (size_t i = 0; i < steps; i++) {
	pixels[(int)round(y) * WIDHT + (int) round(x)] = color;
	x += xinc;
	y += yinc;
    }
}


