#include "rays.h"
#include "lines.h"
#include "inputs.h" 
#include "utils.h"

uint32_t stage[B_WIDHT * B_HEIGHT];
player_t player;
double ticks = 0; 
enum piece { Empty, Wall, Player, Foe};

uint32_t to_height(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, double angle) {
	
	int32_t dx = x1 - x0,
		 dy = y1 - y0;

    	double dist = dx * cos(angle) + dy * sin(angle);
	/* double dist = sqrt(dx*dx + dy*dy); */
	double distSq = dist * dist;

	/* int h =	  map(distSq, 0, WIDHT * HEIGHT, HEIGHT, 0); */
	int h = HEIGHT / dist * BLOCK;
	return h;
}

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

	/* printf("%x\n", key); */

	if (key & Up) {
		player.y += movey;
		player.x += movex;
	}
	else if (key & Down) {
		player.y -= movey;
		player.x -= movex;
	}

	if (key & Left) {
		player.a -= turn_lenght;
	}
	else if (key & Right) {
		player.a += turn_lenght;
	}
	
}

int cast_ray(uint32_t * pixels, player_t player, uint32_t * stage, double angle);
int cast_ray2(uint32_t * pixels, player_t player, uint32_t * stage, double angle, ray ** result);
int casted_line(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t h, uint32_t w, uint32_t i);
uint32_t find_height(size_t index, ray * hits, size_t lenght);
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

/* 	for (size_t i = 0; i < n_lines; i++) { ray hit = hits[i]; uint32_t h = to_height(hit.x0, hit.y0, hit.x1, hit.y1, player.a); /1* uint32_t h = hit.height; *1/ /1* ray nextHit; *1/ /1* if (i != n_lines - 2) *1/ /1* 	nextHit = hits[i + 1]; *1/ /1* else *1/ /1*     	nextHit = hit; *1/ draw_line( */
/* 			pixels, */
/* 			player.x, */
/* 			player.y, */
/* 			hit.x1, */
/* 			hit.y1, */
/* 			0 */
/* 			); */

/* 		casted_line( */  
/* 			&pixels[WIDHT * HEIGHT], */
/* 			player.x, */
/* 			player.y, */
/* 			h, */
/* 			WIDHT / n_lines, */
/* 			i */	
/* 			);			/1* stage[ny * B_WIDHT + nx] = 2; *1/ */
/* 		/1* free(&hits[i]); *1/ */
/* 	} */

	/* free(hits); */
	
	draw_stage(pixels, stage);
	draw_player(pixels, player);

	ticks++;
}


int draw_rect_ycenter(uint32_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t col); 
int draw_trapezoid_ycenter(uint32_t * pixels, uint32_t x, uint32_t y, uint32_t w, uint32_t h0, uint32_t h1, uint32_t col);

uint32_t find_height(size_t index, ray * hits, size_t lenght) {
	
    ray this = hits[index];
    ray upHit, downHit;
    double dist;
    uint32_t up = 0, down = 0;
    bool u = true, d = true;

    for (size_t i = 1 ; u || d ; i++) {
	if (up + down >= 10) break;
	if (d && index - i >= 0) {

	    
	    downHit = hits[index - i];
	    /* dist = sqrt(pow(that.x1 - this.x1, 2) + pow(that.y1 - that.y0, 2)) ; */
	    /* if (dist < BLOCK * i) { */

	    /* } */

	    if (abs((int)downHit.x1 - (int)this.x1) < BLOCK*i || abs((int)downHit.x1 - (int)this.x1) < BLOCK*i)  {
		down++;
	    }
	    else d = 0;
	
	    
	} else d = 0;

	if (u && index + i < lenght - 1) {

	    upHit = hits[index + i];

	    if (abs((int)upHit.x1 - (int)this.x1) < BLOCK*i || abs((int)upHit.x1 - (int)this.x1) < BLOCK*i)  {
		up++;
	    }
	    else u = 0;
	} else u = 0;
    }

    uint32_t hUp =  to_height(upHit.x0, upHit.y0, upHit.x1, upHit.y1, player.a);
    uint32_t hDown =  to_height(downHit.x0, downHit.y0, downHit.x1, downHit.y1, player.a);

    uint32_t h = (hUp + hDown) / 2;
    return h; 
}


int draw_vertline_ycenter(uint32_t * pixels, uint32_t x, uint32_t h, uint32_t col);
int casted_line(uint32_t * pixels, uint32_t x0, uint32_t y0, uint32_t h, uint32_t w, uint32_t i) {

	/* int32_t dx = x1 - x0, */
	/* 	 dy = y1 - y0; */
	/* int32_t dx2 = x2 - x0, */
	/* 	dy2 = y2 - y0; */

	/* //source at https://youtu.be/eOCQfxRQ2pY?t=781 */
    	/* double dist = dx * cos(player.a) + dy * sin(player.a); */
    	/* double dist2 = dx2 * cos(player.a) + dy2 * sin(player.a); */
	/* /1* dist = sqrt(dx * dx + dy * dy); *1/ */

	/* double distSq = dist * dist; */
	/* double distSq2 = dist2 * dist2; */

	/* int h =	  map(distSq, 0, WIDHT * HEIGHT, HEIGHT, 0); */
	/* int h2 =  map(distSq2, 0, WIDHT * HEIGHT, HEIGHT, 0); */
	int col = 0xff * h / HEIGHT;
	/* int bw = to_bw(col); */
	/* printf("%f %d %x %x\n",dist, WIDHT, col, bw); */

	/* uint32_t dh = h2 - h >= 0 ? -(h2 - h) : h2 - h; */

	/* draw_rect_ycenter(pixels, i * w, HEIGHT / 2, w, h, col << 16); */
	draw_vertline_ycenter(pixels, i, h, col << 16); 

	/* draw_trapezoid_ycenter(pixels, i * w, HEIGHT / 2, w, h, h2, col << 16); */

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
int draw_vertline_ycenter(uint32_t * pixels, uint32_t x, uint32_t h, uint32_t col) {

    uint32_t halfHeight = h / 2;
    for (uint32_t yy = HEIGHT / 2 - halfHeight; yy < HEIGHT / 2 + halfHeight; yy++) {
	pixels[yy * WIDHT + x] = col;
    }
    return 0;
}

ray cast_ray5(uint32_t * pixels, player_t player, uint32_t * stage, double dirX, double dirY, double planeX, double planeY, uint32_t x) {

    double cameraX = 2*x / (double)(WIDHT) - 1;
    double rayDirX = dirX + planeX * cameraX,
	   rayDirY = dirY + planeY * cameraX;

    uint32_t posX = player.x;
    uint32_t posY = player.y;
    uint32_t mapX = posX / BLOCK * BLOCK; // posX - (posX % BLOCK)
    uint32_t mapY = posY / BLOCK * BLOCK;
    
    uint32_t offX = posX - mapX;
    uint32_t offY = posY - mapY;

    double sideDistX, sideDistY;

    const double deltaDistX = fabs(1 / rayDirX);
    const double deltaDistY = fabs(1 / rayDirY);

    double perpWallDist;
    
    int stepX, stepY;

    bool hit = false;

    enum Side { NS, EW };
    int side; 

    if (rayDirX < 0) {
	stepX = -1;
	sideDistX = (posX - mapX) * deltaDistX;
    } 
    else {
	stepX = 1;
	sideDistX = (mapX + 1 - posX) * deltaDistX; 
    }
	
    if (rayDirY < 0) {
	stepY = -1;
	sideDistY = (posY - mapY) * deltaDistY;
    } 
    else {
	stepY = 1;
	sideDistY = (mapY + 1 - posY) * deltaDistY; 
    }

    while (!hit) {

	if (sideDistX < sideDistY) {
	    sideDistX += deltaDistX;
	    mapX += stepX;
	    side = 0;	
	}
	else {

	    sideDistY += deltaDistY;
	    mapY += stepY;
	    side = 1;
	}

	if (stage[mapY / BLOCK * B_WIDHT + mapX / BLOCK] != 0) hit = true;
    }


    if (side == 0) 
	    perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
    else
	    perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;
    
    double dist = stepX * cos(player.a) + stepY * sin(player.a);
    uint32_t lineHeight = (uint32_t)(HEIGHT / dist);

    ray r = (ray){ 
	player.x,
	player.y,
	mapX,
	mapY,
	0
    };

/*     draw_line( */
/* 	    pixels, */
/* 	    player.x, */
/* 	    player.y, */
/* 	    mapX, */
/* 	    mapY, */
/* 	    0 */
/* 	    ); */

    uint32_t col = 0xff * lineHeight / HEIGHT;
    printf("%d\n", lineHeight);
    draw_vertline_ycenter(&pixels[HEIGHT * WIDHT], x, lineHeight, col << 16); 

    return r;

	
    
}
ray cast_ray4(uint32_t * pixels, player_t player, uint32_t * stage, int dx, int dy) {

    uint32_t steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double x = player.x;
    double y = player.y;
    uint32_t offX = player.x % BLOCK;
    uint32_t offY = player.y % BLOCK;

    double xinc = (double)(dx) / steps;
    double yinc = (double)(dy) / steps;

    int ix, iy, last_ix, last_iy, newx, newy;
    for (;;) {

	ix = iround(x);
	iy = iround(y);

	if (stage[iy / BLOCK * B_WIDHT + ix / BLOCK] == Wall) {
	
	    newx = ix;
	    newy = iy;

	    if (last_ix == ix) newx += offX;
	    if (last_iy == iy) newy += offY;

	    return (ray) {
		player.x, 
		player.y,
		newx,
		newy,
		0
	    };
	}

	last_ix = ix;
	last_iy = iy;
	x += xinc;
	y += yinc;
    }
 
}
int cast_ray3(uint32_t * pixels, player_t player, uint32_t * stage, double angle, ray ** result) {

    double rayDirX = cos(angle),
	   rayDirY = sin(angle);

    uint32_t posX = player.x;
    uint32_t posY = player.y;
    uint32_t mapX = posX / BLOCK * BLOCK; // posX - (posX % BLOCK)
    uint32_t mapY = posY / BLOCK * BLOCK;
    
    uint32_t offX = posX - mapX;
    uint32_t offY = posY - mapY;

    double sideDistX, sideDistY;

    const double deltaDistX = fabs(1 / rayDirX);
    const double deltaDistY = fabs(1 / rayDirY);

    double perpWallDist;
    
    int stepX, stepY;

    bool hit = false;

    enum Side { NS, EW };
    int side; 
   
    ray * hitRay = malloc(sizeof(ray));

    if (rayDirX < 0) {
	stepX = -1;
	sideDistX = (posX - mapX) * deltaDistX;
    } 
    else {
	stepX = 1;
	sideDistX = (mapX + 1 - posX) * deltaDistX; 
    }
	
    if (rayDirY < 0) {
	stepY = -1;
	sideDistY = (posY - mapY) * deltaDistY;
    } 
    else {
	stepY = 1;
	sideDistY = (mapY + 1 - posY) * deltaDistY; 
    }

    while (!hit) {

	if (sideDistX < sideDistY) {
	    sideDistX += deltaDistX;
	    mapX += stepX;
	    side = 0;	
	}
	else {

	    sideDistY += deltaDistY;
	    mapY += stepY;
	    side = 1;
	}

	if (stage[mapY * B_WIDHT + mapX] != 0) hit = true;
    }


    if (side == 0) 
	    perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
    else
	    perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;
    
    uint32_t lineHeight = (uint32_t)(HEIGHT / perpWallDist);
   
    hitRay->x0 = player.x;
    hitRay->y0 = player.y;
    hitRay->x1 = mapX + offX;
    hitRay->y1 = mapY + offY;
    hitRay->height = lineHeight;

    *result = hitRay;
	
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
    
    

    for (uint32_t ii = 0; ; ii++) {
	

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
	for (double angle = player.a - fov/2; angle <= player.a + fov/2; angle += delta_a, i++)  {
	    	ray hit;

	    	hit = cast_ray5(pixels, player, stage, cos(angle), sin(angle), sin(angle), -cos(angle), i);
		/* memcpy(&hits[i], hit, sizeof(ray)); */
		
		/* hits[i] = hit; */
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

