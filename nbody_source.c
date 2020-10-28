#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include "nbody.h"


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

void body_file_generator(struct body **body_array, int *n_body, char *file_name) {
	FILE *fp = fopen(file_name, "r");
	//Doing an error handling. 
	if(NULL == fp) {
		perror("File Provided does not exist\n");
		fclose(fp);
		return;
	}

	char line[2048];
	int n_line = 0;
	while(fgets(line, 2048, fp)) {
		n_line ++;
	}
	rewind(fp);
	if(0 == n_line) {
		perror("There is nothing in the file");
		fclose(fp);
		return;
	}

	*body_array = (struct body *)malloc(sizeof(struct body) * n_line);
	n_line = 0;

	do {
		fscanf(fp, "%lf,%lf,%lf,%lf,%lf,%lf,%lf", 
		&((*body_array + n_line)->x), &((*body_array + n_line)->y), &((*body_array + n_line)->z),
		&((*body_array + n_line)->velocity_x), &((*body_array + n_line)->velocity_y),
		&((*body_array + n_line)->velocity_z), &((*body_array + n_line)->mass));
		n_line ++;
	} while(fgets(line, 4096, fp));

	*n_body = n_line;

	fclose(fp);

	return;
}

void* velocity_update(void *arg) {
	struct data *data = (struct data *)arg;
	struct body *body_array = data->body_array;
	int n_body = data->n_body;
	double dt = data->dt;
	int thread_index = data->thread_index;

	int st_index = thread_index * (n_body / N_THREAD);
	int end_index = 0;
	if(N_THREAD == (thread_index+1)) {
		end_index = n_body;
	} else {
		end_index = (thread_index + 1) * (n_body / N_THREAD);
	}

	int i = 0;
	int j = 0;
	double xi, yi, zi, xj, yj, zj;
	double a_sumx = 0;
	double a_sumy = 0;
	double a_sumz = 0;
	double massj;
	double distance = 0;
	for(i = st_index; i < end_index; i ++) {
		a_sumx = 0.0;
		a_sumy = 0.0;
		a_sumz = 0.0;
		xi = body_array[i].x;
		yi = body_array[i].y;
		zi = body_array[i].z;
		for(j = 0; j < n_body; j ++) {
			if(i != j) {
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

	return NULL;
}

void* position_update(void *arg) {
	struct data *data = (struct data *)arg;
	struct body *body_array = data->body_array;
	int n_body = data->n_body;
	double dt = data->dt;
	int thread_index = data->thread_index;

	int st_index = thread_index * (n_body / N_THREAD);
	int end_index = 0;
	if(N_THREAD == (thread_index+1)) {
		end_index = n_body;
	} else {
		end_index = (thread_index + 1) * (n_body / N_THREAD);
	}

	for(int i = st_index; i < end_index; i ++) {
		body_array[i].x = body_array[i].x + (body_array[i].velocity_x * dt); 
		body_array[i].y = body_array[i].y + (body_array[i].velocity_y * dt);
		body_array[i].z = body_array[i].z + (body_array[i].velocity_z * dt);
	}
	return NULL;
}

double energy(struct body *body_array, int n_body) {
	double sum = 0.0;
	double sum_inner = 0.0;
	double massi, massj;
	double xi, yi, zi;

	for(int i = 0; i < n_body; i ++) {
		//Velocity squared
		double velocity_2 = pow(body_array[i].velocity_x, 2) + 
			pow(body_array[i].velocity_y, 2) + pow(body_array[i].velocity_z, 2);
		
		massi = body_array[i].mass;

		xi = body_array[i].x;
		yi = body_array[i].y;
		zi = body_array[i].z;
		for(int j = (i + 1); j < n_body; j++) {
			massj = body_array[j].mass;
			sum_inner += (GCONST * massi * massj) / 
				(sqrt(pow(xi - body_array[j].x, 2) + 
				pow(yi - body_array[j].y, 2) + pow(zi - body_array[j].z, 2)));
						
		}
		sum += (0.5*(massi * velocity_2)) - sum_inner;
		sum_inner = 0.0;
	}

	return sum;
}

void step(struct body *body_array, int n_body, double dt) {
	pthread_t threads [N_THREAD];
	struct data data[N_THREAD];

	//Performing the velocity function
	for(int i = 0; i < N_THREAD; i++) {
		data[i].body_array = body_array;
		data[i].n_body = n_body;
		data[i].dt = dt;
		data[i].thread_index = i;
		pthread_create(threads + i, NULL, velocity_update, data + i);
	}
	for(int i = 0; i < N_THREAD; i ++) {
		pthread_join(threads[i], NULL);
	}

	//Performing the position update function
	for(int i = 0; i < N_THREAD; i++) {
		pthread_create(threads + i, NULL, position_update, data + i);
	}
	for(int i = 0; i < N_THREAD; i ++) {
		pthread_join(threads[i], NULL);
	}			
	return;
}

