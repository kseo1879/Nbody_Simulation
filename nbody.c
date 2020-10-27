#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "nbody.h"

/**
 * Body random initializer
 * This will randomly generate the bodies based on the number of bodies (n_body)
 */
void body_rand_generator(struct body *body_array, int n_body) {
	for(int i = 0; i < n_body; i ++) {
		body_array[i].x = (rand() % X_MAX) + ((float)rand())/((float)RAND_MAX) - 1;
		body_array[i].y = (rand() % Y_MAX) + ((float)rand())/((float)RAND_MAX) - 1;
		body_array[i].z = (rand() % Z_MAX) + ((float)rand())/((float)RAND_MAX) - 1;
		body_array[i].velocity_x = (rand() % X_VELOCITY_MAX) + ((float)rand())/((float)RAND_MAX) - 1;
		body_array[i].velocity_y = (rand() % Y_VELOCITY_MAX) + ((float)rand())/((float)RAND_MAX) - 1;
		body_array[i].velocity_z = (rand() % Z_VELOCITY_MAX) + ((float)rand())/((float)RAND_MAX) - 1;
		body_array[i].mass= (rand() % MASS_MAX) + ((float)rand())/((float)RAND_MAX) - 1;
	}
}

void velocity_update(struct body *body_array, int n_body, double dt) {
	int i = 0;
	int j = 0;
	double xi, yi, zi, xj, yj, zj;
	double a_sumx = 0;
	double a_sumy = 0;
	double a_sumz = 0;
	double massj;
	double distance = 0;
	for(i = 0; i < n_body; i ++) {
		a_sumx = 0.0;
		a_sumy = 0.0;
		a_sumz = 0.0;
		xi = body_array[i].x;
		yi = body_array[i].y;
		zi = body_array[i].z;
		for(j = 0; j < n_body; j ++) {
			if(i == j) {
				continue;
			} else {
				xj = body_array[j].x;
				yj = body_array[j].y;
				zj = body_array[j].z;
				massj = body_array[j].mass;
				distance = sqrt(pow(xi - xj, 2) + pow(yi - yj, 2) + pow(zi - zj, 2));
				a_sumx += ((xj - xi) * GCONST * massj)/ pow(distance, 3);
				a_sumy += ((yj - yi) * GCONST * massj)/ pow(distance, 3);
				a_sumz += ((zj - zi) * GCONST * massj)/ pow(distance, 3);
			}
		}
		body_array[i].velocity_x = body_array[i].velocity_x + (a_sumx * dt);
		body_array[i].velocity_y = body_array[i].velocity_y + (a_sumy * dt);
		body_array[i].velocity_z = body_array[i].velocity_z + (a_sumz * dt);
	}

	return;
}

void position_update(struct body *body_array, int n_body, double dt) {
	for(int i = 0; i < n_body; i ++) {
		body_array[i].x = body_array[i].x + (body_array[i].velocity_x * dt); 
		body_array[i].y = body_array[i].y + (body_array[i].velocity_y * dt);
		body_array[i].z = body_array[i].z + (body_array[i].velocity_z * dt);
	}
}

double energy(struct body *body_array, int n_body) {
	double sum = 0.0;
	double sum_inner;
	double massi;
	double xi, yi, zi;

	for(int i = 0; i < n_body; i ++) {
		sum_inner = 0.0;

		//Velocity squared
		double velocity_2 = pow(body_array[i].velocity_x, 2) + 
			pow(body_array[i].velocity_y, 2) + pow(body_array[i].velocity_z, 2);
		massi = body_array[i].mass;

		xi = body_array[i].x;
		yi = body_array[i].x;
		zi = body_array[i].x;
		for(int j = i + 1; j < n_body; j ++) {
			if(i != j) {
				// body_array[j].mass
				sum_inner += (GCONST *  massi) / 
					(sqrt(pow(xi - body_array[j].x, 2) + 
					pow(yi - body_array[j].y, 2) + pow(zi - body_array[j].z, 2)));
			}			
		}
		sum = sum + ((0.5) * (massi * velocity_2)) + sum_inner;
	}
	return sum;
}

void step(struct body *body_array, int n_body, int dt) {
	velocity_update(body_array, n_body, dt);
	position_update(body_array, n_body, dt);
	return;
}

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
	int iteration = 0;
	int n_body = 0;
	float dt = 0.0;
	// char *file_name = NULL;
	struct body *body_array = NULL;
	
	if(5 != argc) {
		perror("Argument is not provided in a correct form\n");
		return -1;
	}

	//Now it will assign to to the function accordingly. 
	if(0 == strcmp(argv[3], "-b")) {
		iteration = (int)atoi(argv[1]);
		dt = (float)atof(argv[2]);
		n_body = (int)atoi(argv[4]);
		body_array = (struct body *)malloc(sizeof(struct body) * n_body);

		// This will randomly generate the bodies
		body_rand_generator(body_array, n_body);
	
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
		// file_name = argv[4];
		
	} else {
		perror("Flags is not defined from the input.");
		return -1;
	}

	return 0;
}
