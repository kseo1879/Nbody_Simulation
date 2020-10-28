#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nbody.h"

/**
 * Main function will first of all set up the bodies based on the user's command line input.
 * 1. If the option is -b than it will randomly generate the bodies
 * 	  If the option is -f it will set up the list accordingly.
 * 2. When the initial set up is done it will call the update velocity function
 * 3. Then it will update the xyz position of all the planets and then finally it will iterate
 * again. 
 * 
 * SUMMARY:
 * Main function will first set the xyz location and xyz velocity of the bodies.
 * Then it will update the velocity
 * Finally it will update the xyz position according to the velocity
 * These steps 
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

	//Now it will assign to to the function accordingly. 
	if(0 == strcmp(argv[3], "-b")) {
		iteration = (int)atoi(argv[1]);
		dt = (double)atof(argv[2]);
		n_body = (int)atoi(argv[4]);
		body_array = (struct body *)malloc(sizeof(struct body) * n_body);

		// This will randomly generate the bodies
		body_rand_generator(body_array, n_body);
	
		//This function will call the step function based on the number of iteration.
		for(int i = 0; i < iteration; i ++) {
			step(body_array, n_body, dt);
		}

		free(body_array);

	} else if(0 == strcmp(argv[3], "-f")) {
		iteration = (int)atoi(argv[1]);
		dt = (double)atof(argv[2]);
		file_name = argv[4];

		body_file_generator(&body_array, &n_body, file_name);

		for(int i = 0; i < iteration; i ++) {
			step(body_array, n_body, dt);
		}

		free(body_array);
		
	} else {
		perror("Flags is not defined from the input.");
		return -1;
	}

	return 0;
}