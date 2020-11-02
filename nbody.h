#ifndef NBODY_H
#define NBODY_H

#define PI (3.141592653589793)
#define MASS (4 * PI * PI)
#define N_DAYS (365.25)
#define GCONST (6.67e-11)

//These are the maximum random values that the planet can have when ramdomly generated
#define X_MAX (70000) 
#define Y_MAX (70000) 
#define Z_MAX (70000) 
#define X_VELOCITY_MAX (10000) 
#define Y_VELOCITY_MAX (10000) 
#define Z_VELOCITY_MAX (10000) 
#define MASS_MAX (1000000) 
#define RAND_NUMER (5.0)

//You can change the number to see the difference of the performance using
//different number of threads. 
#define N_THREAD (4)

/**Size of this body is 8 x 7 which 56 bytes so it might create a false sharing issue
 * When loaded to different cache line. 
 * My systems cache line size was  64 bytes so I added padding down to make the structure 84 byte.
 * 		getconf LEVEL1_DCACHE_LINESIZE
 * 		Above is the command to find the cache size. So depending on the cache size you can change
 * 		The padding
 * body_array[0] (body_array[1]....
 */
struct body {
	double x;
	double y;
	double z;
	double velocity_x;
	double velocity_y;
	double velocity_z;
	double mass;
	double padding;
};

/**
 * This structure is to store the data based on the number of thread that is specified
 * as a macro above
 */
struct data {
	struct body *body_array;
	int n_body;
	double dt;
	int thread_index;
};

/**
 * Body random initializer
 * This will randomly generate the bodies based on the number of bodies (n_body)
 */
void body_rand_generator(struct body *body_array, int n_body);

/**
 * This will generate the bodies based on the data that is saved on the files.
 */
void body_file_generator(struct body **body_array, int *n_body, char *file_name);

/**
 * This will update the velocity based on the value according to the magnitude
 * between different bodies.
 */
void* velocity_update(void *arg);

/**
 * This will update the position of the bodies based on the value of the updated
 * velocity. 
 */
void* position_update(void *arg);

/**
 * This will calculate the energy of the bodies. The value should stay the same.
 * However there can be fluctuation of a value by a small percentage due to the 
 * un-precisedness of the double variable. 
 */
double energy(struct body *body_array, int n_body);

/**
 * This is the step function which will be called based on the iteration that user
 * specified.
 * It will update the x,y,z coordinate of n bodies. 
 */
void step(struct body *body_array, int n_body, double dt, struct data *data);

#endif
