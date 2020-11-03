/**
 * This source code has a simillar behavior as nbody.c
 * However it will print out the initial energy of the bodies and then the energy 
 * of it after the iteration is done
 * If it's less than the DIFF_PERCENTAGE than it will print out that the test has passed
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "nbody.h"

//0.000001 %
//Change this percentage too see how precist the calculation is. 
#define DIFF_PERCENTAGE (0.00000001)

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

	//Now it will assign to to the function accordingly. 
	if(0 == strcmp(argv[3], "-b")) {
		iteration = (int)atoi(argv[1]);
		dt = (double)atof(argv[2]);
		n_body = (int)atoi(argv[4]);
		body_array = (struct body *)malloc(sizeof(struct body) * n_body);

		// This will randomly generate the bodies
		body_rand_generator(body_array, n_body);
		
		double energy_val_prev =  energy(body_array, n_body);
		printf("Energy Constant Before the iteration: %f\n\n",energy_val_prev);

		struct data data[N_THREAD];
		for(int i = 0; i < N_THREAD; i ++) {
			data[i].body_array = body_array;
			data[i].n_body = n_body;
			data[i].dt = dt;
			data[i].thread_index = i;
		}

		//This function will call the step function based on the number of iteration.
		for(int i = 0; i < iteration; i ++) {
			//This step function is declaed under nbody.h
			//and it is implemented on nbody_source.c
			step(body_array, n_body, dt, data);
		}

		//Printing the energy val after the iteration
		double energy_val_after = energy(body_array, n_body);
		printf("Energy Constant After the iteration: %f\n\n", energy_val_after);

		//Getting the absolute value of energy difference. 
		double energy_difference = sqrt(pow(energy_val_after - energy_val_prev, 2));

		//Also getting the absolute value of the energy after 
		energy_val_after = sqrt(pow(energy_val_after, 2));

		if(energy_difference <= energy_val_after * DIFF_PERCENTAGE) {
			printf("Passed the test: Fluctuation was less than %f%%\n", DIFF_PERCENTAGE * 100);
		} else {
			printf("Energy Value Fluctuated Too much. Higher than the goal %f\n", DIFF_PERCENTAGE * 100);
		}

		free(body_array);

	} else if(0 == strcmp(argv[3], "-f")) {

		iteration = (int)atoi(argv[1]);
		dt = (double)atof(argv[2]);
		file_name = argv[4];
		body_file_generator(&body_array, &n_body, file_name);
		
		//=== DEREFERENCE THIS LINE TO SEE THE IF INITIALIZED CORRECTLY===
		// for(int n_line = 0; n_line < n_body; n_line++) {
		// 	printf("%d: %lf,%lf,%lf,%lf,%lf,%lf,%lf\n", n_line,
		// 	body_array[n_line].x, body_array[n_line].y, body_array[n_line].z,
		// 	body_array[n_line].velocity_x, body_array[n_line].velocity_y, body_array[n_line].velocity_z, 
		// 	body_array[n_line].mass);
		// }

		//Checking the enrgy value before the iteration
		double energy_val_prev =  energy(body_array, n_body);
		printf("Energy Constant Before the iteration: %f\n\n",energy_val_prev);

		struct data data[N_THREAD];
		for(int i = 0; i < N_THREAD; i ++) {
			data[i].body_array = body_array;
			data[i].n_body = n_body;
			data[i].dt = dt;
			data[i].thread_index = i;
		}

		//Based on the number of iteration it will call step functions.
		for(int i = 0; i < iteration; i ++) {
			//This step function is declaed under nbody.h
			//and it is implemented on nbody_source.c
			step(body_array, n_body, dt, data);
			//=== DEREFERENCE THIS LINE TO SEE THE THE VALUE CHANGING===
			// printf("Position of First body: \n");
			// printf("x:%f\ty:%f\tz:%f\n", body_array[1].x, body_array[1].y, body_array[1].z);
			// printf("Energy Constant : %f\n\n", energy(body_array, n_body));
		}

		//Printing the energy val after the iteration
		double energy_val_after = energy(body_array, n_body);
		printf("Energy Constant After the iteration: %f\n\n", energy_val_after);

		//Getting the absolute value of energy difference. 
		double energy_difference = sqrt(pow(energy_val_after - energy_val_prev, 2));

		//Also getting the absolute value of the energy after 
		energy_val_after = sqrt(pow(energy_val_after, 2));

		//If the energy difference is less than the percentage than it will pass the test
		if(energy_difference <= energy_val_after * DIFF_PERCENTAGE) {
			printf("Passed the test: Fluctuation was less than %f%%\n", DIFF_PERCENTAGE * 100);
		} else {
			printf("Energy Value Fluctuated Too much. Higher than the goal %f\n\n\n", DIFF_PERCENTAGE * 100);
		}

		free(body_array);
		
	} else {
		perror("Flags is not defined from the input.");
		return -1;
	}

	return 0;
}
