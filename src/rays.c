#include "rays.h"
#include "lines.h"
#include "utils.h" 
#include "inputs.h" 
#include "castlines.h" 

uint32_t stage[B_WIDHT * B_HEIGHT];
player_t player;
double ticks = 0; 
enum piece { Empty, Wall, Player, Foe};


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

int draw_player(uint32_t * pixels, player_t player) {
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

void player_movement() {
	int key = get_keypress();
	double movement_lenght = 2.0;
	double turn_lenght = 0.05;
	double movex, movey;
	
	movey = round(movement_lenght * sin(player.a));
	movex = round(movement_lenght * cos(player.a));

	switch (key) {

	    case Up:
		player.y += movey;
		player.x += movex;
		break;
	    case Down:
		player.y -= movey;
		player.x -= movex;
		
		break;
	    case Right:
		player.a += turn_lenght;
		break;
	    case Left:
		player.a -= turn_lenght;
		break;
	}
}

int cast_ray(uint32_t * pixels, player_t player, uint32_t * stage, double angle);
int cast_ray2(uint32_t * pixels, player_t player, uint32_t * stage, double angle, ray ** result);
int casted_line(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t w, uint32_t i);
#define NLINES WIDHT
ray hits[NLINES];
void loop(uint32_t * pixels) {
    	
	create_stage(pixels, stage);
	player_movement();

	ray hitPoint;	

	/* cast_ray2(pixels, player, stage, M_PI_4, &hitPoint); */
	/* draw_line(pixels, player.x, player.y, hitPoint.x1, hitPoint.y1, 0); */
	uint32_t n_lines = NLINES;
	/* ray * hits = malloc(n_lines * sizeof(ray)); */

	cast_rays(pixels, player, stage, hits, n_lines, M_PI_2);

	for (size_t i = 0; i < n_lines; i++) {
		ray hit = hits[i];
		ray nextHit;
		if (i != n_lines - 2) 
			nextHit = hits[i + 1];
		else 
		    	nextHit = hit;
		draw_line(
			pixels,
			player.x,
			player.y,
			hit.x1,
			hit.y1,
			0
			);

		casted_line(  
			&pixels[WIDHT * HEIGHT],
			player.x,
			player.y,
			hit.x1,
			hit.y1,
			nextHit.x1,
			nextHit.y1,
			WIDHT / n_lines,
			i	
			);			/* stage[ny * B_WIDHT + nx] = 2; */
		/* free(&hits[i]); */
	}

	/* free(hits); */
	
	draw_stage(pixels, stage);
	draw_player(pixels, player);

	ticks++;
}


int draw_rect_ycenter(uint32_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t col); 
int draw_trapezoid_ycenter(uint32_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h0, uint32_t h1, uint32_t col);

int casted_line(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t x2, uint32_t y2, uint32_t w, uint32_t i) {

	int32_t dx = x1 - x0,
		 dy = y1 - y0;
	int32_t dx2 = x2 - x0,
		dy2 = y2 - y0;

	//source at https://youtu.be/eOCQfxRQ2pY?t=781
    	double dist = dx * cos(player.a) + dy * sin(player.a);
    	double dist2 = dx2 * cos(player.a) + dy2 * sin(player.a);
	/* dist = sqrt(dx * dx + dy * dy); */

	double distSq = dist * dist;
	double distSq2 = dist2 * dist2;

	int h =	  map(distSq, 0, WIDHT * HEIGHT, HEIGHT, 0);
	int h2 =  map(distSq2, 0, WIDHT * HEIGHT, HEIGHT, 0);
	int col = map(dist * dist, 0, WIDHT * WIDHT, 0xff, 0);
	/* int bw = to_bw(col); */
	/* printf("%f %d %x %x\n",dist, WIDHT, col, bw); */

	uint32_t dh = h2 - h >= 0 ? -(h2 - h) : h2 - h;

	/* draw_rect_ycenter(pixels, i * w, HEIGHT / 2, w, h, col << 16); */

	draw_trapezoid_ycenter(pixels, i * w, HEIGHT / 2, w, h, h2, col << 16);

	/* if (dh == 0 || dh > 3*BLOCK) */
	/* 	draw_rect_ycenter(pixels, i * w, HEIGHT / 2, w, h, col << 16); */
	/*     	/1* draw_trapezoid_ycenter(pixels, i * w, HEIGHT / 2, w, h, h, col << 16); *1/ */
	/* else */ 
	/*     	draw_trapezoid_ycenter(pixels, i * w, HEIGHT / 2, w, h, h2, col << 16); */
	

	return 0;
}

int draw_trapezoid_ycenter(uint32_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h0, uint32_t h1, uint32_t col) {
	
    	double h02 = h0 / 2;
	double h12 = h1 / 2;
	double dh = h12 - h02;
	double slope = dh / w;

	uint32_t h2;

	/* if (dh == 0) { */
	/*     return draw_rect_ycenter(pixels, x, y, w, h0, col); */
	/* } */

	

	for (size_t xx = x ; xx < x + w; xx++) {

	    h2 = h02 + round(slope * (xx - x) );

	    for (size_t yy = y - h2; yy < y + h2; yy++) {
	    
		    pixels[yy * WIDHT + xx] = col;
	    }
	}
    
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

int cast_ray2(uint32_t * pixels, player_t player, uint32_t * stage, double angle, ray ** result) {

    uint32_t x = player.x,
	     y = player.y; 
    uint32_t step_size = BLOCK;
    ray * hit = malloc(sizeof(ray));
    int stepx = round(step_size * cos(angle));
    int stepy = round(step_size * sin(angle));
    for (;;) {

	if (stage[y / BLOCK * B_WIDHT + x / BLOCK] != 0) {
	    //hit
	    hit->x0 = player.x;
	    hit->y0 = player.y;
	    hit->x1 = x;
	    hit->y1 = y;

	    *result = hit;
	    /* free(hit); */
	    return 1;
	}

	x += stepx;
	y += stepy;
    }
    /* free(hit); */
    return 0;
}

int cast_ray(uint32_t * pixels, player_t player, uint32_t * stage, double angle) {
	
    uint32_t x = player.x, 
	     y = player.y;
    uint32_t xTile = player.x % BLOCK,
	     yTile = player.y % BLOCK;
    
    // 1st quadrant
	
    double tana = tan(angle);
    /* int32_t dy = -yTile; */
    /* int32_t dx = -yTile / tana; */
    double yStep = 1 / tana; // step on x-axis 
    double xStep = -tana; // step on y-axis 

    int32_t tileStepX = BLOCK;
    int32_t tileStepY = -BLOCK;

    double yIntercept = player.y + xTile / tana;
    double xIntercept = player.x + -yTile / tana; 
    
    
    printf("init %d %f %d %f %f\n", player.x, xIntercept, yTile, tana, yTile / tana);

    for (uint32_t ii = 0; ; ii++) {
	
	printf("0 %d %f %f\n", ii, xIntercept, xStep);

	if (stage[(int)round(yIntercept / BLOCK) * B_WIDHT + x / BLOCK] != 0) {
	    draw_line(pixels, player.x, player.y, x, yIntercept, 0);
	    return 1; // hit vertical
	}
	x += tileStepX; 
	yIntercept += yStep;

	if (stage[y / BLOCK * B_WIDHT + (int)round(xIntercept / BLOCK)] != 0) {
	    draw_line(pixels, player.x, player.y, xIntercept, y, 0);
	    return 1; // hit horizontal
	}
	y += tileStepY;
	xIntercept += xStep;
    }
    
}

int cast_rays(uint32_t * pixels, player_t player, uint32_t * stage, ray * rays, uint32_t n_lines, double fov) {
	
	ray * hits = rays;
	double delta_a = fov / (n_lines + 1);
	uint32_t i = 0;
	for (double angle = player.a - fov/2; angle <= player.a + fov / 2; angle += delta_a, i++)  {
	    	ray * hit = NULL;
	    	cast_ray2(pixels, player, stage, angle, &hit);
		memcpy(&hits[i], hit, sizeof(ray));
	}
	return 0;
}



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
					break;
				case Foe:
					draw_block(pixels, x, y, 0xffff00, 0xffff00);
					break;
			}
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

