#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nbody.h"

/**
 * Main function will first of all set up the bodies based on the user's command line input.
 * 1. If the option is -b than it will randomly generate the bodies
 * 	  If the option is -f it will set up the list accordingly.
 * 2. When the initial set up is done it will call the step function
 * 	(1). Inside the step function it will first update the velocity and then update the p
 * 		position of the bodies.  
 * 	
 * SUMMARY:
 * Main function will first set the xyz location and xyz velocity of the bodies.
 * (based on the flag that user has specified)
 * Then it will update the velocity (based on the magnitude between different bodies)
 * Finally it will update the xyz position according to the velocity
 * It will continue to do this based on the number of the iteration.
 */
int main(int argc, char** argv) {
	
	//This are the values that will store the user input value. 
	struct body *body_array = NULL;
	int iteration = 0;
	int n_body = 0;
	double dt = 0.0;
	char *file_name = NULL;

	//Error handling to see if the user has put in the correct argument.
	if(5 != argc) {
		perror("Argument is not provided in a correct form\n");
		return -1;
	}

	//Now it will check which flag user has specified either -b or -f 
	if(0 == strcmp(argv[3], "-b")) {
		iteration = (int)atoi(argv[1]);
		dt = (double)atof(argv[2]);
		n_body = (int)atoi(argv[4]);
		body_array = (struct body *)malloc(sizeof(struct body) * n_body);

		// This will randomly generate the bodies
		body_rand_generator(body_array, n_body);

		//This structure is defined under nbody.h
		//This data will be passed to the step function which each thread will have 
		//their own specified data.  will 
		struct data data[N_THREAD];
		for(int i = 0; i < N_THREAD; i ++) {
			data[i].body_array = body_array;
			data[i].n_body = n_body;
			data[i].dt = dt;
			data[i].thread_index = i;
		}
	
		//This function will call the step function based on the number of iteration.
		for(int i = 0; i < iteration; i ++) {
			step(body_array, n_body, dt, data);
		}

		free(body_array);

	//This means that it should be reading from the file
	} else if(0 == strcmp(argv[3], "-f")) {

		//Store the user specified data.
		iteration = (int)atoi(argv[1]);
		dt = (double)atof(argv[2]);
		file_name = argv[4];

		//Generate the bodies based on the value stored from the file
		body_file_generator(&body_array, &n_body, file_name);

		//This structure is defined under nbody.h
		//This data will be passed to the step function which each thread will have 
		//their own specified data. 
		struct data data[N_THREAD];
		for(int i = 0; i < N_THREAD; i ++) {
			data[i].body_array = body_array;
			data[i].n_body = n_body;
			data[i].dt = dt;
			data[i].thread_index = i;
		}

		//Call the step function based on the number of iteration.
		for(int i = 0; i < iteration; i ++) {
			step(body_array, n_body, dt, data);
		}

		free(body_array);
		
	} else {
		//Error state.
		perror("Flags should be either -b or -f\n");
		return -1;
	}

	return 0;
}