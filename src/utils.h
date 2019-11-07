#ifndef RAY_UTILS_H
#define RAY_UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <math.h>




/* int get_arrow_press() { */
/* 	char input[10]; */
/* 	if (fgets(input, sizeof(input), stdin) != NULL) { */

/* 	    size_t len = strlen(input); */
/* 	    /1* for (size_t i = 0; i < len; i++) *1/ */ 
/* 	    if (len < 3) return NoPress; */
/* 	    if (input[0] == '\33') { */
/* 		switch (input[2]) { */

/* 		    case 'A': */
/* 			return Up; */
/* 			break; */
/* 		    case 'B': */
/* 			return Down; */
/* 			break; */
/* 		    case 'C': */
/* 			return Right; */
/* 			break; */
/* 		    case 'D': */
/* 			return Left; */
/* 			break; */
			
/* 		} */
/* 	    } */

/* 	} */
/* 	return NoPress; */ 
/* } */

long map(long value, long input_start, long input_end, long output_start, long output_end) {
    double slope = 1.0 * (output_end - output_start) / (input_end - input_start);  
    return  output_start + slope * (value - input_start);
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
