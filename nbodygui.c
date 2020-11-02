#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "nbody.h"

//This MAX_VALUE is a double(*2) of maximum value of bodies position. 
//If some body has maximum x initial value of 1000, and MAX_VALUE would be 2000
//However it is better to make it big enough to correctly see them at the gui
#define MAX_VALUE (250000000000.0)
#define CIRCLE_COLOUR 0xFF0000FF

int main(int argc, char** argv) {

	//Error handling
	if (argc != 8) {
		perror("The command line argument is not in correct form\n");
		perror("GUI should have total 7 arguments excluding the execution file\n");
		return -1;
	}

	//Setting the width and height of the gui
	int view_width = atoi(argv[1]);
	int view_height = atoi(argv[2]);

	//Setting the number of iteration based on the argument.
	//When it has finished the iteration it will terminate the gui
	int iteration = atoi(argv[3]);
	double dt = (double)atof(argv[4]);

	//Scale Default value Should be 1. It will zoom in and zoom out
	//based on the vale. (ex: scale = 2, will zoom in twice).
	//From the middle of the value.
	double scale = strtod(argv[7], NULL);


	int n_body;
	struct body* body_array;

	//Checking which flag user has entered. 
	//(Either -b or -f flag)
	if(0 == strcmp(argv[5], "-b")) {

		n_body = atoi(argv[6]);
		body_array = (struct body *)malloc(sizeof(struct body) * n_body);

		//This function is declaed in 'nbody.h'
		//It will randomly generate the bodies and save it to the body_array
		body_rand_generator(body_array, n_body);
	
	} else if(0 == strcmp(argv[5], "-f")) {

		char *file_name = argv[6];

		//This function is declaed in 'nbody.h'
		//It will generate the bodies based the data that is store on file_name
		body_file_generator(&body_array, &n_body, file_name);
		
	} else {
		//Error, flag not in correct form.
		perror("The flag should either be -b or -f");
		return -1;
	}

	//Initializing the data. This structure is declaed under nbody.h.
	//This is to ensure that threads has different data. 
	struct data data[N_THREAD];
	for(int i = 0; i < N_THREAD; i ++) {
		data[i].body_array = body_array;
		data[i].n_body = n_body;
		data[i].dt = dt;
		data[i].thread_index = i;
	}

	//Setting up the windows. 
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;
	int finished = 0;
	
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
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		view_width, view_height, SDL_WINDOW_SHOWN);

	//Error handling to see if it has succesfully implemented the windows. 
	if(NULL == window) {
		printf("SDL_CreateWindow returned NULL: %s\n", SDL_GetError());
		free(body_array);
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_ACCELERATED 
		| SDL_RENDERER_PRESENTVSYNC);

	int iteration_count = 0; 
	double scale_xy = view_height/(MAX_VALUE * 2);

	//This is the while loop that will iterate based on the nubmer of iteration that 
	//user has provided. 
	while(!finished) {


		//This is to ensure that the program terminates after it has finisehd executing
		if(iteration_count == iteration) {
			finished = (event.type == SDL_QUIT);
			break;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		
		SDL_RenderClear(renderer);
		
		//First draw the bodies based on the number
		for (int i = 0; i < n_body; i++) {
			struct body* body = body_array+i;
			filledCircleColor(renderer, ((body->x * scale_xy) * scale) + (view_width * 0.5),
				view_height - (((view_height * 0.5) + ((body->y * scale_xy)* scale))),  scale * 7, CIRCLE_COLOUR);
		}

		SDL_RenderPresent(renderer);

		//Call teh step function the update the velocity and to update it's position	
		step(body_array, n_body, dt, data);

 		if(SDL_PollEvent(&event)) {
			finished = (event.type == SDL_QUIT);
		}
		iteration_count ++;
	}
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();	
	free(body_array);

	return 0;	
}
