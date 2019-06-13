#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "rays.h"
int main(int argc, char** argv) {
	
	bool leftMouseButtonDown = false;
	bool quit = 0;
	SDL_Event event;
	uint32_t screen_size = SCREEN_SIZE * sizeof(uint32_t);
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow(
			"My Window!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDHT, HEIGHT, 0);
	
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture * texture = SDL_CreateTexture(
			renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, WIDHT, HEIGHT);

	uint32_t pixels[SCREEN_SIZE];
	memset(pixels, 255, screen_size);
 	while (!quit) {
		SDL_UpdateTexture(texture, NULL, pixels, WIDHT * sizeof(uint32_t));
		
		SDL_WaitEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
		}
		draw(pixels);		
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}

			/* case SDL_MOUSEBUTTONUP: */
			/* 	if (event.button.button == SDL_BUTTON_LEFT) */
			/* 	    leftMouseButtonDown = false; */
			/* 	break; */
		    	/* case SDL_MOUSEBUTTONDOWN: */
			/* 	if (event.button.button == SDL_BUTTON_LEFT) */
			/* 	    leftMouseButtonDown = true; */
			/* case SDL_MOUSEMOTION: */
			/* 	if (leftMouseButtonDown) { */
			/* 	    int mouseX = event.motion.x; */
			/* 	    int mouseY = event.motion.y; */
			/* 	    pixels[mouseY * 640 + mouseX] = 0; */
			/* 	} */
			/* 	break; */
