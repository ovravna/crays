#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL2/SDL.h"
#include "game.h"
#include "inputs.h"

int main(int argc, char** argv) {
	
	bool quit = 0;
	SDL_Event event;
	uint32_t screen_size = SCREEN_SIZE * sizeof(uint32_t);
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window * window = SDL_CreateWindow(
			"CRAYS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDHT, SCREEN_HEIGHT, 0);
	
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture * texture = SDL_CreateTexture(
			renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDHT, SCREEN_HEIGHT);

	uint32_t pixels[SCREEN_SIZE];
	uint32_t bg_color = 0xff;
	memset(pixels, bg_color, screen_size);
	setup(pixels);
 	while (!quit) {
		memset(pixels, bg_color, screen_size);
		loop(pixels);		
		SDL_UpdateTexture(texture, NULL, pixels, WIDHT * sizeof(uint32_t));
		
		SDL_PollEvent(&event);
		switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				/* keysDown(event.key.keysym.sym); */
				switch (event.key.keysym.sym) {
				    case SDLK_UP: 
					keypress(Up);
					break;
				    case SDLK_DOWN: 
					keypress(Down);
					break;
				    case SDLK_RIGHT: 
					keypress(Right);
					break;
				    case SDLK_LEFT: 
					keypress(Left);
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				    case SDLK_UP: 
					keyup(Up);
					break;
				    case SDLK_DOWN: 
					keyup(Down);
					break;
				    case SDLK_RIGHT: 
					keyup(Right);
					break;
				    case SDLK_LEFT: 
					keyup(Left);
					break;
				}
				break;

		}
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
