#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "nbody.h"

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
			printf("Position of First body: \n");
			printf("x:%f\ty:%f\tz:%f\n", body_array[0].x, body_array[0].y, body_array[0].z);
			printf("Energy Constant : %f\n\n", energy(body_array, n_body));
		}

		free(body_array);

		// struct body body[5] = { {.x = 0.0, .y = 0.0, .z = 0.0, 
		// 	.velocity_x = 0.0, .velocity_y = 0.0, .velocity_z = 0.0, .mass = 1988999999999999901909255192576.0}, 
		// 	{.x = 150000000000.0, .y = 0.0, .z = 0.0, 
		// 	.velocity_x = 0.0, .velocity_y = 29800.0, .velocity_z = 0.0, .mass = 5974000000000000373293056.0},
		// 	{.x = 230000000000.0, .y = 0.0, .z = 0.0, 
		// 	.velocity_x = 0, .velocity_y = 24100.0, .velocity_z = 0.0, .mass = 641899999999999963299840.0},
		// 	{.x = 55000000000.0, .y = 0.0, .z = 0.0, 
		// 	.velocity_x = 0, .velocity_y = 47900.0, .velocity_z = 0.0, .mass = 330199999999999993708544.0},
		// 	{.x = 100000000000.0, .y = 0, .z = 0.0, 
		// 	.velocity_x = 0.0, .velocity_y = 35000.0, .velocity_z = 0.0, .mass = 4869000000000000115343360.0} 
		// };

		// position_update(body, n_body, dt);

		// for(int i = 0; i < iteration; i ++) {
		// 	step(body, n_body, dt);
		// 	printf("Position of First body: \n");
		// 	printf("x:%f\ty:%f\tz:%f\n", body[1].x, body[1].velocity_y, body[1].z);
		// 	printf("Energy Constant : %f\n\n", energy(body, n_body));
		// }

	} else if(0 == strcmp(argv[3], "-f")) {
		iteration = (int)atoi(argv[1]);
		dt = (double)atof(argv[2]);
		file_name = argv[4];
		body_file_generator(&body_array, &n_body, file_name);
        printf("dt : %f\n\n", dt);
		// for(int n_line = 0; n_line < n_body; n_line++) {
		// 	printf("%d: %lf,%lf,%lf,%lf,%lf,%lf,%lf\n", n_line,
		// 	body_array[n_line].x, body_array[n_line].y, body_array[n_line].z,
		// 	body_array[n_line].velocity_x, body_array[n_line].velocity_y, body_array[n_line].velocity_z, 
		// 	body_array[n_line].mass);
		// }

		for(int i = 0; i < iteration; i ++) {
			step(body_array, n_body, dt);
			printf("Position of First body: \n");
			printf("x:%f\ty:%f\tz:%f\n", body_array[1].x, body_array[1].y, body_array[1].z);
			printf("Energy Constant : %f\n\n", energy(body_array, n_body));
		}

		free(body_array);
		
	} else {
		perror("Flags is not defined from the input.");
		return -1;
	}

	return 0;
}
