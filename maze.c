#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include "maze.h"

/* global variables */
pthread_t* thread_handles;
pthread_mutex_t mutex;
long thread_count, cursor;
MAZE* gen_maze, sol_maze; /* one global variable for initializing the maze, another for solving */

/* adds the given edge to the next slot in the edges
	NOTE: needs a mutex surrounding it like fish need water -- NOT THREAD SAFE */
int push_back(PAIR new_edge)
{
	int verts = (int) pow(*gen_maze.side_length, 2); /* size is number of vertices */
	if (cursor < verts-1)
	{
		(*gen_maze.edges)[cursor] = new_edge;
		cursor++;
		return 0;
	} else {
		return 1;
	}
}

/* generates a random pair returned through the given pointer, or null if no valid vertex can be found */
void rand_edge(int size, int* p1, int* p2) 
{
	if (size <= 1)
		return NULL;
	int num, counter, row, col;
	row = (int) floor(*p1/size);
	col = *p1 % size;
	num = rand() % 4;
	counter = 0;
	while (counter < 4)
	{
		/* checks if p2 is a valid point and has not been visited*/
		if (num == 0 && row > 0 && !(*gen_maze.vertices[(row-1)*size + col]))
		{
			*p2 = (row-1)*size + col;
			break;
		} else if (num == 1 && *p1.y < size-1 && !(*gen_maze.vertices[row*size + (col+1)])) {
			*p2 = row*size + (col+1);
			break;
		} else if (num == 2 && *p1.x < size-1 && !(*gen_maze.vertices[(row+1)*size + col])) {
			*p2 = (row+1)*size + col;
			break;
		} else if (num == 3 && *p1.y > 0 && !(*gen_maze.vertices[row*size + (col-1)])) {
			*p2 = row*size + (col-1);
			break;
		} else {
			num = (num+1) % 4;
			counter++;
		}
	}
	return;
}

void *threadGenerate(void *my_rank)
{
	long rank, n, my_n;
	rank = (long) my_rank;
	int i, row_comp, col_comp, *my_maze;
	i = 0;
	n = (long) pow(*gen_maze.side_length,2);
	my_n = n/thread_count;

	/* add code to generate portion of maze */
	while (i < my_n -1)
	{

	}

    /* using tree-structured reduction */
    int iter = 0;
    while (1)
    {
        if ((rank % (int) pow(2,iter+1)) == 0) {
            if (rank == 0 && (int) pow(2,iter) == comm_size) {
                /* maze is fully generated, exit process */
                pthread_exit(0);
            }
            /* wait for thread to complete its maze generation */
            pthread_join(thread_handles[(int) rank+pow(2,iter)], NULL);

            /* resolve appropriate connection between 2 parts of maze */
        	
        } else {
            pthread_exit(0);
        }
        iter++;
    }
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
    long i;
    double start_time, end_time;
    thread_count = num_threads;
    cursor = 0;
    thread_handles = malloc(num_threads*sizeof(pthread_t));
    pthread_mutex_init(&mutex);

    /* make maze m global variable and initialize the size, vertices, and edge variables */
    gen_maze = m;
    *maze.side_length = size;
    *maze.edges = malloc((pow(size,2)-1)*sizeof(PAIR));
    *maze.vertices = malloc(pow(size,2)*sizeof(int));

    start_time = getProcessTime();
    /* Start the threads. */
    for (thread = 0; thread < threads; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       threadGenerate, (void*) thread);

    /* Wait for thread 0 to finish, since thread 0 ends
    *  after all the other threads have completed */
    pthread_join(thread_handles[0], NULL);

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