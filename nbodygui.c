#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#define VIEW_WIDTH (640)
#define VIEW_HEIGHT (480)

//THIS IS A SDL2 TEMPLATE

/**
 * Updates the x based on where it is, will invert the  
 * velocity when it reaches out of bounds.
 */
void update_x(int* cx, int* velocity, int width) {
	if(cx && velocity) {
		if(*cx <= 0) {
			*cx = 0;
			*velocity = 1;
		} else if(*cx >= width) {
			*cx = width;
			*velocity = -1;
		}
		*cx = *cx + *velocity;
	}
}

int main(int argc, char** argv) {

	//Your SDL variables and variables for circle data and finished state
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;
	int finished = 0;
	int circle_x = 100;
	int vel = 1;
	
	//Initialises SDL Video Acceleration
	//Check if return value is < 0
	SDL_Init(SDL_INIT_VIDEO);
	
	/**
	 * Creates a window to display
	 * Allows you to specify the window x,y position
	 * Allows you to specify the width and height of the window
	 * Option for flags for the window and potentially the different instances
	 * you may want to use with it. SHOWN, makes the window visible
	 * Should check if it returns NULL
	 */
	window = SDL_CreateWindow("SDL Template",
		10, 10,
		VIEW_WIDTH, VIEW_HEIGHT, SDL_WINDOW_SHOWN);
	if(NULL == window) {
		printf("Something Wrong?%s\n", SDL_GetError());
		return -1;
	}
	
	/**
	 * Create Renderer retrieves the 2D rendering context of the window
	 * You will need to use this to use with SDL2_gfx library 
	 * This will give you access to the underlying rederer of the window
	 * Should check if it returns NULL
	 */
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC);
	
	/**
	 * Render loop of your application
	 * You will perform your drawing in this loop here
	 */
	while(!finished) {
		//Sets the background colour 
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		
		//Clears target with a specific drawing colour (prev function defines colour)
		SDL_RenderClear(renderer);
		
		//Draws a circle using a specific colour
		//Pixel is RGBA (0x(RED)(GREEN)(BLUE)(ALPHA), each 0-255
		filledCircleColor(renderer, circle_x, 100, 7, 0xFF0000FF);
		
		//Updates the screen with newly renderered image
		SDL_RenderPresent(renderer);
			
		//Updates the x coordinate of circle
		update_x(&circle_x, &vel, VIEW_WIDTH);

		//Retrieves the events captured from SDL (just watching for windows close)
		if(SDL_PollEvent(&event)) {
			finished = (event.type == SDL_QUIT);
		}		
	}
	
	//Clean up functions
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();	

	return 0;	
}
