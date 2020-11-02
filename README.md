# N-Body Simulation

This is the submittion for N-Body Simulation SOFT3410 Assignment1

By: Calvin Seo
SID: 480347192

## How to use

There are total three options to build this project.
1. To just run the program (Without anything showing) (i.e: ./nbody)
2. To test the energy value with command line argument (i.e: ./test)
3. To run and see with GUI (i:e ./nbody-gui)

### 1. To just run the program 

To compile: You can simply type this in your command line
```bash
make nbody
```

Now, You have two options here to test the performance of the program using the following code: (after you compile using the command above)

```bash
make performance_test_file
```
OR
```bash
make performance_test_rand
```

This will test the performance of the program.
Typing those command will automatically run something like this:
(time ./nbody 50000 0.1 -f ./CSV/solar.csv)

- HOWEVER, you can run your own test after you compile just like this
- ./nbody iteration dt (-b nbody | -f file path)

### 2. To test the energy value with command line argument

To Compile: You can simply type this in your command line
```bash
make test
```

Now, You have three options here to test the energy value. It will compare the energy value between the start of the iteration and at the end of the iteration(after you compile using the command above)

```bash
make energy_test
```
OR
```bash
make energy_test2
```
OR
```bash
make energy_test3
```

- HOWEVER, you can run your own test after you compile just like this"
- ./test iteration dt (-b nbody | -f file path)

The test output would be something like this:
```bash
./nbody_gui 800 800 500000 1000 -f ./CSV/solar.csv 1

Energy Constant Before the iteration: -6709904628577069066283610317783040.000000

Energy Constant After the iteration: -6709904628581853690527728732733440.000000

Passed the test: Fluctuation was less than 0.000100%
```
First it prints out the energy constant before the iteration and one that after the iteration. It will pass the test when the difference is less than the percentage specified. 

- To change this value please change the macro on nbodytest.c DIFF_PERCENTAGE value. (line 17)

### 3. To run and see with GUI

To compile: You can simply type this in your command line
```bash
make nbody-gui
```

Now, You have three options here to see different gui

```bash
make solar_gui
```
OR
```bash
make solar2_gui
```
OR
```bash
make rand_gui
```

First one will run the solar.csv file under CSV folder. Second one will run solar2.csv file under csv folder. The last one will show the randomly generated bodies. 

- HOWEVER, you can run your own test after you compile just like this"
- (./nbody-gui resolution_width resolution_height iteration dt (-b nbody | -f file path) scale)

## To clean the repository

Simply type this in the command line
```bash
make clean
```

## To change the Number of threads
To change the number of threads please look at nbody.h file.

If you look at line 21 of the file there is a macro N_THREAD.

You need to clean the repository after you change the value and rebuild it.
