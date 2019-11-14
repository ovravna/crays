#include "game.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL_keycode.h>
#include "inputs.h"
#include "textures.h"

#define mapWidth 24
#define mapHeight 24

uint32_t stage[mapWidth][mapHeight]=
{
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
  {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

vec pos, dir, plane;

double posX = 22, posY = 12, dirX = -1, dirY = 0, planeX = 0, planeY = 0.66;

void draw_vertline(uint32_t * pixels, uint32_t x, uint32_t h, uint32_t col) {

    if (h > HEIGHT) h = HEIGHT;
    uint32_t h2 = h / 2;
    uint32_t H2 = HEIGHT / 2;

    for (uint32_t y = H2 - h2; y < H2 + h2; y++) {
	pixels[y * WIDHT + x] = col;
    }
}
void draw_vertline_tex(uint32_t * pixels, uint32_t x, uint32_t h, uint32_t * texture, uint32_t tex_w, uint32_t index) {

    if (h > HEIGHT) h = HEIGHT;
    uint32_t h2 = h / 2;
    uint32_t H2 = HEIGHT / 2;

    for (uint32_t y = H2 - h2, i = 0; y < H2 + h2; y++, i++) {
	uint32_t off = (tex_w) * i / h;
	pixels[y * WIDHT + x] = texture[off * tex_w + index];
    }
}

long mapping(long value, long input_start, long input_end, long output_start, long output_end) {
    double slope = 1.0 * (output_end - output_start) / (input_end - input_start);  
    return  output_start + slope * (value - input_start);
}

void setup(uint32_t * pixels) {
    pos = (vec) { 22, 12 };
    dir = (vec) { -1, 0 };
    plane = (vec) { 0, 0.66 };
    /* double posX = 22, posY = 12, dirX = -1, dirY = 0, planeX = 0, planeY = 0.66; */

}

void inputHandler();
uint32_t idx = 0;
void loop(uint32_t * pixels) {

	for (uint32_t x = 0; x < WIDHT; x++) {
	     	     //calculate ray position and direction
	      double cameraX = 2 * x / (double)(WIDHT) - 1; //x-coordinate in camera space
	      double rayDirX = dirX + planeX * cameraX;
	      double rayDirY = dirY + planeY * cameraX;
	      //which box of the map we're in
	      int mapX = ( int )(posX);
	      int mapY = ( int )(posY);

	      double offX = posX - mapX;
	      double offY = posY - mapY;

	      //length of ray from current position to next x or y-side
	      double sideDistX;
	      double sideDistY;

	       //length of ray from one x or y-side to next x or y-side
	      const double deltaDistX = fabs(1 / rayDirX);
	      double deltaDistY = fabs(1 / rayDirY);
	      double perpWallDist;

	      //what direction to step in x or y-direction (either +1 or -1)
	      int stepX;
	      int stepY;

	      int hit = 0; //was there a wall hit?
	      int side; //was a NS or a EW wall hit?
	      //calculate step and initial sideDist
	      if (rayDirX < 0)
	      {
		stepX = -1;
		sideDistX = (posX - mapX) * deltaDistX;
	      }
	      else
	      {
		stepX = 1;
		sideDistX = (mapX + 1.0 - posX) * deltaDistX;
	      }
	      if (rayDirY < 0)
	      {
		stepY = -1;
		sideDistY = (posY - mapY) * deltaDistY;
	      }
	      else
	      {
		stepY = 1;
		sideDistY = (mapY + 1.0 - posY) * deltaDistY;
	      }
	      //perform DDA
	      while (hit == 0)
	      {
		//jump to next map square, OR in x-direction, OR in y-direction
		if (sideDistX < sideDistY)
		{
		  sideDistX += deltaDistX;
		  mapX += stepX;
		  side = 0;
		}
		else
		{
		  sideDistY += deltaDistY;
		  mapY += stepY;
		  side = 1;
		}
		//Check if ray has hit a wall
		if (stage[mapX][mapY] > 0) hit = 1;
	      }

	      //Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
	      if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2) / rayDirX;
	      else           perpWallDist = (mapY - posY + (1 - stepY) / 2) / rayDirY;

	      //Calculate height of line to draw on screen
	      int lineHeight = HEIGHT / perpWallDist;

	      uint32_t col = 0xff;

	      switch (stage[mapX][mapY]) {
		  case 1: 
		      col = col << 16;
		      break;
		  case 2: 
		      col = (col << 8);
		      break;
		  case 3: 
		      col = col & 0xff;
		      break;
		  case 4: 
		      col = (col << 16) | (col << 8) ;
		      break;
		  case 5: 
		      col = (col << 8) | (col) ;
		      break;
	      }

	      if (side) col &= 0x7f7f7f;
	

	
	     /* uint32_t d = (uint32_t) dist; */
	     /* printf("%d\n", d); */ 
	     /* int lineHeight = mapping(d, 0, HEIGHT, HEIGHT - 10, 0); */ 


	      uint32_t n = 64;


	      //calculate value of wallX
	      double wallX; //where exactly the wall was hit
	      if (side == 0) wallX = posY + perpWallDist * rayDirY;
	      else           wallX = posX + perpWallDist * rayDirX;
	      wallX -= floor((wallX));

	      uint32_t index =  (wallX * n);

	      if(side == 0 && rayDirX > 0) index = n - index - 1;
	      if(side == 1 && rayDirY < 0) index = n - index - 1;
	      /* idx++; */
	      /* idx %= n; */
	      draw_vertline_tex(&pixels[WIDHT * HEIGHT], x, lineHeight, *dog, n, index);


	}

	inputHandler();
}


void keysDown(int key) {

}

void inputHandler() {
	
    int key = get_keypress(); 

    double rotSpeed = 0.02;
    double moveSpeed = 0.03;

    if (key & Up) {
      if(stage[( int )(posX + dirX * moveSpeed)][( int )(posY)] == false) posX += dirX * moveSpeed;
      if(stage[( int )(posX)][( int )(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
    }

    if (key & Down) {
      if(stage[( int )(posX - dirX * moveSpeed)][( int )(posY)] == false) posX -= dirX * moveSpeed;
      if(stage[( int )(posX)][( int )(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;

    }
    
    if (key & Right) {
	double oldDirX = dirX;
        dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
	dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
	double oldPlaneX = planeX;
	planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
	planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);

    }

    if (key & Left) {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);

    }

}


	/* double cameraX = 2.0 * x / (double)WIDHT - 1; //x-coordinate in camera space */
	/*      vec rayDir = (vec) { */
	/* 	  dir.x + plane.x * cameraX, */ 
	/* 	  dir.y + plane.y * cameraX */ 
	/*      }; */

	/*      ivec map = (ivec) { (int)(pos.x), (int)(pos.y) }; */
	/*      vec sideDist; */
	/*      vec deltaDist = (vec) { */
	/* 	 fabs(1 / rayDir.x), fabs(1 / rayDir.y) */
	/*      }; */
	 
	/*      double dist; */

	/*      ivec step; */

	/*      bool hit = false; */

	/*      int side; */

	/*      if (rayDir.x < 0) */
	/*      { */
	/* 	step.x = -1; */
	/* 	sideDist.x = (pos.x - map.x) * deltaDist.x; */
	/*      } */
	/*      else */
	/*      { */
	/* 	step.x = 1; */
	/* 	sideDist.x = (map.x + 1.0 - pos.x) * deltaDist.x; */
	/*      } */
	/*      if (rayDir.y < 0) */
	/*      { */
	/* 	step.y = -1; */
	/* 	sideDist.y = (pos.y - map.y) * deltaDist.y; */
	/*      } */
	/*      else */
	/*      { */
	/* 	step.y = 1; */
	/* 	sideDist.y = (map.y + 1.0 - pos.y) * deltaDist.y; */
	/*      } */

	/*      while (hit == 0) */
	/*      { */
	/* 	//jump to next map square, OR in x-direction, OR in y-direction */
	/* 	if (sideDist.x < sideDist.y) */
	/* 	{ */
	/* 	  sideDist.x += deltaDist.x; */
	/* 	  map.x += step.x; */
	/* 	  side = 0; */
	/* 	} */
	/* 	else */
	/* 	{ */
	/* 	  sideDist.y += deltaDist.y; */
	/* 	  map.y += step.y; */
	/* 	  side = 1; */
	/* 	} */
	/* 	//Check if ray has hit a wall */
	/* 	if (stage[map.x][map.y] > 0) hit = true; */
	/*      } */
	     
	/*      if (side == 0) dist = (map.x - pos.x + (1 - step.x) / 2) / rayDir.x; */
	/*      else           dist = (map.y - pos.y + (1 - step.y) / 2) / rayDir.y; */

