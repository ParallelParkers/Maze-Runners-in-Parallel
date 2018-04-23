/* Parallel Computing Project: Maze Generation and Algorithmic Solving Efficiency Analysis 
	Team Members:  Danilo Baledio, Naomi Nunis, Katya Sheth, Mark Fraser */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "maze.h"

#define MAX_THREADS 256
#define MAX_SIZE    256

/* declaration of functions used in main() */
void processCommandLineArgs(int argc 	 			/* in */,
							char* argv[] 			/* in */,
							long* num_threads 		/* out */,
							long* maze_size   		/* out */
							);

/* main takes the command line arguments and calls upon the functions in maze.c */
int main(int argc, char* argv[])
{
	long num_threads, maze_size, algorithm_choice;
	processCommandLineArgs( argc,
							argv,
							&num_threads,
							&maze_size
						  );

	/* TODO:  call the appropriate function to generate and return a maze (using pointer reference in argument)
	i.e. */
	MAZE maze;
	generateMaze(&maze, maze_size, num_threads);

	/* TODO:  call appropriate function to solve maze and generate output .csv file */
	// printMaze(&maze);
	return 0;
}

void processCommandLineArgs(int argc, char* argv[], long* num_threads, long* maze_size)
{
	/* parse out parameters for maze generation and solving */
	if (argc != 3) 
	{
		fprintf(stderr, "Incorrect usage of program.  Proper usage is:\n    ./maze <num_threads> <maze_size>\nexiting...\n");
		exit(1);
	}

	*num_threads = (long) atoi(argv[1]);
	*maze_size   = (long) atoi(argv[2]);

	if (*num_threads < 1 || *num_threads > MAX_THREADS)
		fprintf(stderr, "Invalid input for number of threads. Number must be between 1 and %d", MAX_THREADS);
	if (*maze_size < 2 || *maze_size > MAX_SIZE || *maze_size < *num_threads)
		fprintf(stderr, "Invalid input for size of maze. Number must be between 2 and %d", MAX_SIZE);
}