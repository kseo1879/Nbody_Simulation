#ifndef NBODY_H
#define NBODY_H

#define PI (3.141592653589793)
#define MASS (4 * PI * PI)
#define N_DAYS (365.25)
#define GCONST (6.67e-11)
#define X_MAX (700) 
#define Y_MAX (700) 
#define Z_MAX (700) 
#define X_VELOCITY_MAX (10) 
#define Y_VELOCITY_MAX (10) 
#define Z_VELOCITY_MAX (10) 
#define MASS_MAX (20) 
#define RAND_NUMER (5.0)
#define N_THREAD (4)

/**Size of this body is 8 x 7 which 56 bytes so it might create a false sharing issue
 * When loaded to different cache line. 
 * My systems cache line size was  64 bytes so I added padding down to make the structure 84 byte.
 * 		getconf LEVEL1_DCACHE_LINESIZE
 * 		Above is the command to find the cache size. So depending on the cache size you can change
 * 		The padding
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

void body_file_generator(struct body **body_array, int *n_body, char *file_name);

void* velocity_update(void *arg);

void* position_update(void *arg);

double energy(struct body *body_array, int n_body);

/**
 * This is the step function which will be called based on the iteration that user
 * specified.
 * It will update the x,y,z coordinate of n bodies. 
 */
void step(struct body *body_array, int n_body, double dt);

#endif
