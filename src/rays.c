#include "rays.h"
#include "lines.h"
 
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
	player.a += 0.001;
	player.x = (uint32_t) xd;
	player.y = (uint32_t) yd;
	cast_rays(pixels, player, stage, M_PI/2);
	stage[player.y * B_WIDHT + player.x] = 2;
	draw_stage(pixels, stage);
	/* printf("%d %d\n", player.x, player.y); */
	ticks++;
}


void draw_line_shitty(uint32_t * pixels,  int x0, int y0, int x1, int y1, uint32_t color);

int cast_rays(uint32_t * pixels, player_t player, uint32_t * stage, double fov) {
	double delta_a = fov / 20;
	for (double angle = player.a - fov/2; angle < player.a + fov / 2; angle += delta_a)  {
		bool hit = false;
		double hx = cos(angle); 
		double hy = sin(angle);
		uint32_t nx; 
		uint32_t ny; 
		/* printf("Hit stuff: %f %f\n", hx, hy); */
		for (uint32_t n = 1; !hit; n++) {
			nx = player.x + n * hx;
			ny = player.y + n * hy; 
			if (stage[ny * B_WIDHT + nx] != 0) {
				hit = true;
				draw_line(
					pixels,
					player.x*BLOCK + BLOCK / 2,
					player.y*BLOCK + BLOCK / 2,
					nx*BLOCK + BLOCK / 2,
					ny*BLOCK + BLOCK /2,
					0
					);
				/* stage[ny * B_WIDHT + nx] = 2; */
				/* printf("Hit stuff: %d %d\n", nx, ny); */
				break;
			}
			if (ny >= B_HEIGHT || nx >= B_WIDHT || ny == 0 || nx == 0) {
				hit = true;
				printf("Hit wall: %d %d\n", nx, ny);
				break;
			}
			/* stage[ny * B_WIDHT + nx] = 3; */
			/* printf("None: %d %d\n", nx, ny); */
		}
		hit = false;
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
					draw_block(pixels, x, y, 0xff0000, 0); 
					break;
				case 2:
					draw_player(pixels, x, y);
					break;
				case 3:
					draw_block(pixels, x, y, 0xffff00, 0xffff00);
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

void draw_line4(uint32_t * pixels,  int x0, int y0, int x1, int y1) {
	double dx = x1 - x0;
	double dy = y1 - y0;
	double D = 2*dy -dx;
	if (dx == 0 || dy == 0) return; 
	int y = y0;
	int signx = dx < 0 ? -1 : 1;
	int signy = dy < 0 ? -1 : 1;
	for (uint32_t x = x0; x != x1; x += signx) {
		pixels[y * WIDHT + x] = 0;
		if (D > 0) {
			y += signy;
			D -= 2*dx;
		}
		D += 2*dy;
	}

}
void draw_line3(uint32_t * pixels,  int x0, int y0, int x1, int y1) {
	uint32_t color = 0;
	/* if (x0 > x1) { */
	/* 	int xtmp = x1; */
	/* 	x1 = x0; */ 
	/* 	x0 = xtmp; */
	/* } */
	
	/* if (y0 > y1) { */
	/* 	int ytmp = y1; */
	/* 	y1 = y0; */ 
	/* 	y0 = ytmp; */
	/* } */
	double deltax = x1 - x0;
	double deltay = y1 - y0;

	int signx = deltax < 0 ? -1 : 1;
	int signy = deltay < 0 ? -1 : 1;

	if (deltax == 0) {
		for (uint32_t y = y0; y != y1; y += signy) {
			pixels[y * WIDHT + x0] = color;
		}
		return;
	}
	double deltaerr = fabs(deltay/deltax);
	double err = 0;
	uint32_t y = y0;
	for (uint32_t x = x0; x != x1; x += signx) {

		pixels[y * WIDHT + x] = 0;
		err += deltaerr;
		if (err >= 0.5) {
			y += signy;
			err -= 1.0;
		}
	}
	

}
void draw_line_shitty(uint32_t * pixels,  int x0, int y0, int x1, int y1, uint32_t color) {
	/* uint32_t lenght = sqrt( pow(x1 - x0, 2) + pow(y1 - y0, 2) ); */
	double step = 40;

	if (x1 - x0 == 0) {
		double dy = (y1 - y0) / step;
		if (x1 - x0 == 0) {

			for (uint32_t y = y0, n = 0; n < step; y += dy, n++) {

				if (y < 0 || y >= HEIGHT || x0 < 0 || x0 >= WIDHT) {
					/* printf("Outside %d %d\n", x, y); */
				} else { 
					pixels[y * WIDHT + x0] = color;
				}
			}
		}
		return;
	}
	double a = (1.0*y1 - y0)/(x1 - x0);
	double dx = (x1 - x0) / step; 
	for (int x = x0, n = 0; n < step; n++, x += dx) {
		uint32_t y =  (a * (x - x0) + y0);
		if (y < 0 || y >= HEIGHT || x < 0 || x >= WIDHT) {
			/* printf("Outside %d %d\n", x, y); */
		} else { 
			pixels[y * WIDHT + x] = color;
		}
	}
} 

int draw_block(uint32_t* pixels, uint32_t x_block, uint32_t y_block, uint32_t color, uint32_t bg_color) {
	
	for (uint32_t y = 0; y < BLOCK;y++) {
		for (uint32_t x = 0; x < BLOCK;x++) {
			uint32_t c = color;
			if (y == 0 || y == BLOCK - 1 || x == 0 || x == BLOCK - 0) {
				c = bg_color;	
			}
			
			pixels[(y_block*BLOCK + y)*WIDHT + x_block*BLOCK + x] = c;
		}
	}
	return 0;  
}

