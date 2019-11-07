#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>


long map(long value, long input_start, long input_end, long output_start, long output_end) {
    return  output_start + ((output_end - output_start) / (input_end - input_start)) * (value - input_start);
}

uint32_t to_bw(uint32_t col) {
	uint32_t r = col >> 16,
		 g = (col >> 8) & 0xff,
		 b = col & 0xff;

	uint32_t avg = (r + g + b) / 3;
	avg = avg & 0xff;
	uint32_t res = (avg << 16) | (avg << 8) | avg;
	printf("%x %x %x | %x | %x\n", r, g, b, avg, res);
	return res; 

}

#endif
