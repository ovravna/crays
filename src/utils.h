#ifndef RAY_UTILS_H
#define RAY_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

long map(long value, long input_start, long input_end, long output_start, long output_end) {
    double slope = 1.0 * (output_end - output_start) / (input_end - input_start);  
    return  output_start + slope * (value - input_start);
}

int iround(double value) {
    return (int) round(value);
}

uint32_t to_bw(uint32_t col) {
	uint32_t avg;
    	if (col <= 0xff) {
		avg = col;
	} else {

	    uint32_t r = col >> 16,
		     g = (col >> 8) & 0xff,
		     b = col & 0xff;

	    avg = (r + g + b) / 3;
	    avg = avg & 0xff;
	}
	uint32_t res = (avg << 16) | (avg << 8) | avg;
	/* printf("%x %x %x | %x | %x\n", r, g, b, avg, res); */
	return res; 

}

#endif
