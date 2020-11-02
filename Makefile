CC=gcc
CFLAGS=-g -std=c11 -Wall -Werror 
.PHONY: clean


#COMPILING
nbody: nbody.c nbody_source.c
	$(CC) $(CFLAGS) $^ -o $@ -fsanitize=address -lpthread -lm 

nbody-gui: nbody_source.c nbodygui.c  
	$(CC) $(CFLAGS) $^ -o $@ -lpthread -lm -lSDL2 -lSDL2_gfx 

test: nbodytest.c nbody_source.c
	$(CC) $(CFLAGS) $^ -o $@ -fsanitize=address -lpthread -lm


#####################################################
#TESTING
## Here it will test the performance of the code
performance_test_file:
	time ./nbody 50000 0.1 -f ./CSV/solar.csv

performance_test_rand:
	time ./nbody 1000 0.1 -b 400


#####################################################
## This will test the fluctuation of the energy value
energy_test: 
	./test 10000 0.1 -f ./CSV/solar.csv

energy_test2: 
	./test 50000 0.1 -f ./CSV/planets.csv

energy_test3: 
	./test 50000 0.1 -b 10


#####################################################
## From here it will test if the gui is working. 
solar_gui:
	./nbody-gui 800 800 500000 500 -f ./CSV/solar.csv 1

solar2_gui:
	./nbody-gui 800 800 500000 10000 -f ./CSV/solar2.csv 1

rand_gui:
	./nbody-gui 800 800 100000 5000 -b 10 1


#####################################################
## To clean the repository
clean:
	rm -f *.o
	rm -f nbody_gui
	rm -f nbody
	rm -f test
