#include "rays.h"
#include <sys/queue.h>
 
uint32_t stage[B_WIDHT * B_HEIGHT];
player_t player;
double ticks = 0; 
int create_stage(uint32_t * pixels, uint32_t * stage) { /* if (stage != NULL) return 0; */
	
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
	player = (player_t) { 
		.x = B_WIDHT / 2,
		.y = B_HEIGHT / 2,
		.a = 0
	};
	create_stage(pixels, stage);
}
void loop(uint32_t * pixels) {
	create_stage(pixels, stage);
	double xd = cos(ticks/4000) * B_WIDHT / 5 + B_WIDHT / 2;
	double yd = sin(ticks/4000) * B_HEIGHT / 5 +  B_HEIGHT / 2;
	player.x = (uint32_t) xd;
	player.y = (uint32_t) yd;
	cast_rays(pixels, player, stage, M_PI/2);
	stage[player.y * B_WIDHT + player.x] = 2;
	draw_stage(pixels, stage);
	/* printf("%d %d\n", player.x, player.y); */
	ticks++;
}


int cast_rays(uint32_t * pixels, player_t player, uint32_t * stage, double fov) {
	double delta_a = fov / 20;
	for (double angle = player.a - fov/2; angle < fov / 2; angle += delta_a)  {
		bool hit = false;
		double hx = cos(angle); 
		double hy = sin(angle);
		uint32_t n = 1;
		/* printf("Hit stuff: %f %f\n", hx, hy); */
		for (uint32_t i = 0; i < 10 && !hit; i++, n++) {
			uint32_t nx = player.x + n * hx;
			uint32_t ny = player.y + n * hy; 
			if (stage[ny * B_WIDHT + nx] != 0) {
				hit = true;
				/* draw_line(pixels, player.x, player.y, nx, ny); */
				stage[ny * B_WIDHT + nx] = 2;
				printf("Hit stuff: %d %d\n", nx, ny);
				break;
			}
			if (ny >= B_HEIGHT || nx >= B_WIDHT || ny == 0 || nx == 0) {
				hit = true;
				printf("Hit wall: %d %d\n", nx, ny);
				break;
			}
			/* printf("None: %d %d\n", nx, ny); */
		}
	}
	return 0;
}

int draw_stage(uint32_t* pixels, uint32_t* stage) {
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

void draw_line(uint32_t * pixels,  uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) {
	uint32_t lenght = sqrt( pow(x1 - x0, 2) + pow(y1 - y0, 2) );
	double a = (y1 - y0)/(x1 - x0);
	for (uint32_t x = x0; x < x1; x++) {
		pixels[ (uint32_t) (a * x + y0) ] = 0;
		pixels[ (uint32_t) (a * x + y0 - 1) ] = 0;
		pixels[ (uint32_t) (a * x + y0 + 1) ] = 0;
		// TODO: fix 
	}
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

