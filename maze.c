#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "maze.h"

/* global variables */
long thread_count;
MAZE* maze;

void *threadWord(void *my_rank)
{
	long rank = (long) my_rank;

	/* add code to generate portion of maze */

	/* add code to send, receive, and join portion of maze */
}

void generateMaze(MAZE* maze, long size, long num_threads)
{
	/* add code to create pthreads */

	/* add code to join pthreads */
}

void solveMaze(MAZE* maze, long alg)
{
	/* add code to solve maze (perhaps create separate functions for each algorithm?) */
}