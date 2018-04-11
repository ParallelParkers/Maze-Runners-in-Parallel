#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include "maze.h"

/* global variables */
long thread_count, cursor;
MAZE* gen_maze, sol_maze; /* one global variable for initializing the maze, another for solving */

/* adds the given edge to the next slot in the edges*/
int push_back(PAIR new_edge)
{
	size = pow(*gen_maze.side_length, 2); /* size is number of vertices */
	if (cursor == size-2)
	{
		(*gen_maze.edges)[cursor] = new_edge;
		cursor++;
		return 0;
	} else
		return 1;
}

/* generates a random pair returned through the given pointer */
void rand_edge(int size, PAIR* p1, PAIR* p2) 
{
	if (size <= 1)
		return NULL;
	int num;
	*p1.x = rand() % size;
	*p1.y = rand() % size;
	num = rand() % 4;
	while (1)
	{
		if (num == 0 && *p1.x > 0)
		{
			*p2.x = *p1.x-1;
			*p2.y = *p1.y;
			break;
		} else if (num == 1 && *p1.y < size-1) {
			*p2.x = *p1.x;
			*p2.y = *p1.y+1;
			break;
		} else if (num == 2 && *p1.x < size-1) {
			*p2.x = *p1.x+1;
			*p2.y = *p1.y;
			break;
		} else if (num == 3 && *p1.y > 0) {
			*p2.x = *p1.x;
			*p2.y = *p1.y-1;
			break;
		} else {
			num = (num+1) % 4;
		}
	}
}

void *threadGenerate(void *my_rank)
{
	long rank = (long) my_rank;

	/* add code to generate portion of maze */

	/* add code to send, receive, and join portion of maze */
}

void *threadSolve(void *my_rank)
{
	long rank = (long) my_rank;
	double start_time, end_time;
	char buf[32];

	start_time = getProcessTime();

	if (rank == 0)
	{
		/* random mouse algorithm */
		buf = "random mouse"
	} else if (rank == 1)
	{
		/* breadth-first search */
		buf = "breadth-first search"
	} else if (rank == 2)
	{
		/* depth-first search */
		buf = "depth-first search"
	} else if (rank == 3)
	{
		/* right-hand solution */
		buf = "right-hand"
	} else
	{
		/* error message */
		fprintf(stderr, "Incorrect number of threads passed into threadSolve() function.\nexiting...\n");
		exit(0);
	}

	end_time = getProcessTime();
	printf("Time to complete %s algorithm: %f", buf, end_time-start_time);
}

void generateMaze(MAZE* m, long size, long num_threads)
{
	/* add code to create pthreads */
    long       thread;
    pthread_t* thread_handles;
    long i;
    double start_time, end_time;
    cursor = 0;
    thread_handles = malloc(num_threads*sizeof(pthread_t));

    /* make maze m global variable and initialize the size, vertices, and edge variables */
    gen_maze = m;
    *maze.side_length = size;
    *maze.edges = malloc((pow(size,2)-1)*sizeof(PAIR));
    *maze.vertices = malloc(size*sizeof(int*));
    for (int i=0; i<size; i++)
    	*(*maze.vertices) = malloc(size*sizeof(int));

    start_time = getProcessTime();
    /* Start the threads. */
    for (thread = 0; thread < threads; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       threadGenerate, (void*) thread);

    /* Wait for threads to finish. */
    for (thread = 0; thread < threads; thread++)
        pthread_join(thread_handles[thread], NULL);

    end_time = getProcessTime();

    printf("Time to generate maze: %f\n", end_time-start_time);
}

void solveMaze(MAZE* m)
{
	/* add code to create pthreads */
    long       thread;
    pthread_t* thread_handles;
    long i;
    thread_handles = malloc(num_threads*sizeof(pthread_t));

    /* make maze m global variable and initialize the size*/
    gen_maze = m;
    (*maze)->side_length = size;

    /* Start the threads. */
    for (thread = 0; thread < threads; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       threadGenerate, (void*) thread);

    /* Wait for threads to finish. */
    for (thread = 0; thread < threads; thread++)
        pthread_join(thread_handles[thread], NULL);
}

void printMaze(MAZE* m)
{
	/* add code to print maze contents */
}