#include "rays.h"
#include "lines.h"
#include "utils.h" 

uint32_t stage[B_WIDHT * B_HEIGHT];
player_t player;
double ticks = 0; 
enum piece { Empty, Wall, Player, Foe};

typedef struct s {
	uint32_t x0, y0, x1, y1;
} ray;

size_t n_rays = 100;
ray rays[100];

int create_stage(uint32_t * pixels, uint32_t * stage) { /* if (stage != NULL) return 0; */
	
	/* stage = melloc(B_WIDHT * B_HEIGHT * sizeof(uint32_t)); */
	
	for (uint32_t y = 0; y < B_HEIGHT; y++) {
		for (uint32_t x = 0; x < B_WIDHT; x++) {
			if (x == 0 || x == B_WIDHT - 1 || y == 0 || y == B_HEIGHT - 1)
				stage[y * B_WIDHT + x] = Wall;
			else if ((y == 4 && x > 0 && x < 6) || (y == 12 && x < B_WIDHT && x > B_WIDHT - 8)) 
				stage[y * B_WIDHT + x] = Wall;
			else 
				stage[y * B_WIDHT + x] = Empty;
		}
	}
	
	return 0;
}
void setup(uint32_t * pixels) {
	player = (player_t) { 
		.x = WIDHT / 2,
		.y = HEIGHT / 2,
		.a = 0
	};
	create_stage(pixels, stage);
}

int draw_player2(uint32_t * pixels, player_t player) {
	uint32_t r = BLOCK / 2;		
   		
	int cx, cy;
	for (size_t yy = player.y - r ; yy < player.y + r; yy++) {
	    for (size_t xx = player.x - r ; xx < player.x + r; xx++) {
		cx = player.x - xx;
		cy = player.y - yy;
		double d = sqrt(cx*cx + cy*cy);
		if (d < r) 
		    pixels[yy * WIDHT + xx] = 0x00ff00;
	    }
	}
	return 0;
}

void loop(uint32_t * pixels) {
    	
	create_stage(pixels, stage);
	double xd = cos(ticks/1000) * WIDHT / 3 + WIDHT / 2;
	double yd = sin(ticks/1000) * HEIGHT / 3 + HEIGHT / 2;
	player.a += M_PI / 2 * 0.001;
	player.x = (uint32_t) xd;
	player.y = (uint32_t) yd;
	cast_rays(pixels, player, stage, M_PI/2);
	/* stage[player.y * B_WIDHT + player.x] = 1; */
	draw_stage(pixels, stage);
	draw_player2(pixels, player);
	/* printf("%d %d\n", player.x, player.y); */
	ticks++;
}


int draw_rect_ycenter(uint32_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t col); 

int casted_line(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t w, uint32_t i) {
    	/* for (uint32_t yy = 0; yy < HEIGHT; yy++) { */
	    /* for (uint32_t xx = 0; xx < WIDHT; xx++) { */
		/* pixels[yy * WIDHT + xx] = 0; // map(yy * WIDHT + xx, 0, HEIGHT * WIDHT, 0, 0xffffff); */
	    /* } */
	/* } */
	int32_t dx = x1 - x0,
		 dy = y1 - y0;
	//source at https://youtu.be/eOCQfxRQ2pY?t=781
    	double dist = dx * cos(player.a) + dy * sin(player.a);

	/* double dist = sqrt(dx * dx + dy * dy); */
	//todo map dist to height
	int h =	  map(dist, 0, WIDHT, HEIGHT, 0);
	int col = map(dist * dist, 0, WIDHT * WIDHT, 0xff, 0);
	int bw = to_bw(col);
	/* printf("%f %d %x %x\n",dist, WIDHT, col, bw); */
	//todo map dist to color
	//todo draw rects
	
	draw_rect_ycenter(pixels, i * w, HEIGHT / 2, w, h, col << 16);
	

	return 0;
}

int draw_rect_ycenter(uint32_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t col) {
	
    	uint32_t h2 = h / 2;

	for (size_t yy = y - h2; yy < y + h2; yy++) {
	    for (size_t xx = x; xx < x + w; xx++) {
		pixels[yy * WIDHT + xx] = col;
	    }
	}
    
	return 0;
}

int cast_rays2(uint32_t * pixels, player_t player, uint32_t * stage, double fov) {

}

int cast_rays(uint32_t * pixels, player_t player, uint32_t * stage, double fov) {
    	uint32_t n_lines = 100;
	double delta_a = fov / n_lines;
	uint32_t i = 0;
	for (double angle = player.a - fov/2; angle <= player.a + fov / 2; angle += delta_a, i++)  {
		double hx = cos(angle); 
		double hy = sin(angle);
		uint32_t nx; 
		uint32_t ny; 

		for (uint32_t n = 1 ;; n++) {
			nx = player.x / BLOCK + n * hx;
			ny = player.y / BLOCK + n * hy; 
			if (stage[ny * B_WIDHT + nx] != 0) {
			    	/* rays[i] = (ray) { */
					/* player.x*BLOCK + BLOCK / 2, */
					/* player.y*BLOCK + BLOCK / 2, */
					/* nx*BLOCK + BLOCK / 2, */
					/* ny*BLOCK + BLOCK /2 */
				/* }; */


				draw_line(
					pixels,
					player.x,
					player.y,
					nx*BLOCK + BLOCK / 2,
					ny*BLOCK + BLOCK /2,
					0
					);

				casted_line(   // todo: drawing should definetly not happen here...
					&pixels[WIDHT * HEIGHT],
					player.x,
					player.y,
					nx*BLOCK + BLOCK / 2,
					ny*BLOCK + BLOCK /2,
					WIDHT / n_lines,
					i	
					);			/* stage[ny * B_WIDHT + nx] = 2; */
				break;
			}
			if (ny >= B_HEIGHT || nx >= B_WIDHT || ny == 0 || nx == 0) {
				printf("Hit wall: %d %d\n", nx, ny);
				break;
			}
		}
	}
	return 0;
}

/* int cast_ray(); */


int draw_stage(uint32_t* pixels, uint32_t* stage) {
	for (uint32_t y = 0; y < B_HEIGHT; y++) {
		for (uint32_t x = 0; x < B_WIDHT; x++) {
			switch (stage[y*B_WIDHT + x]) {
				case Empty:
					/* draw_block(pixels, x, y, 0xffffff, 0xffffff); */
					break;
				case Wall:
					draw_block(pixels, x, y, 0xff0000, 0); 
					break;
				case Player:
					/* draw_block(pixels, x, y, 0xffffff, 0xffffff); */
					draw_player(pixels, x, y);
					break;
				case Foe:
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

