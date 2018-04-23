#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>
#include "maze.h"
#include "timer.h"

/* global variables */
pthread_t* thread_handles;
pthread_mutex_t mutex;
long thread_count, cursor;
MAZE* gen_maze, sol_maze; /* one global variable for initializing the maze, another for solving */

/* adds the given edge to the next slot in the edges
	NOTE: needs a mutex surrounding it like fish need water -- NOT THREAD SAFE */
int push_back(PAIR new_edge)
{
	int verts = (int) pow(gen_maze->side_length, 2); /* size is number of vertices */
	if (cursor < verts-1)
	{
		(gen_maze->edges)[cursor] = new_edge;
		cursor++;
		return 0;
	} else {
		return 1;
	}
}

/* generates a random pair returned through the given pointer, or null if no valid vertex can be found */
void rand_edge(long size, long width, long height, int* p1, int* p2) 
{
	if (size <= 1)
	{
		*p2 = -1;
		return;
	}
	int num, counter, row, col;
	row = (int) floor(*p1/width);
	col = *p1 % width;
	num = rand() % 4;
	counter = 0;

	while (counter < 4)
	{
		/* checks if p2 is a valid point and has not been visited*/
		if (num == 0 && row > 0)
		{
			*p2 = (row-1)*width + col;
			return;
		} else if (num == 1 && col < width-1) {
			*p2 = row*width + (col+1);
			return;
		} else if (num == 2 && row < height-1) {
			*p2 = (row+1)*width + col;
			return;
		} else if (num == 3 && col > 0) {
			*p2 = row*width + (col-1);
			return;
		} else {
			num = (num+1) % 4;
			counter++;
		}
	}
	/* could not find random edge... returning null */
	*p2 = -1;
	return;
}

int visited(int x, int size, int* maze)
{
	for (int i=0; i<size; i++) 
		if (x == maze[i])
			return 1;
	return 0;
}

void *threadGenerate(void *my_rank)
{
	long rank, n, my_n;
	PAIR pair;
	int i, a, b, row_comp, side, col_comp, comp, cursor, *my_maze;
	i = 0;
	side = gen_maze->side_length;
	cursor = 1;
	rank = (long) my_rank;
	n = (long) pow(side,2);
	my_n = n/thread_count;
	comp = (int) my_n*rank;
	// row_comp = (int) (n/(thread_count/2))*floor(rank/2);
	// col_comp = (int) (n/2)*(rank % 2);
	my_maze = &((gen_maze->vertices)[comp]);
	my_maze[0] = rand() % my_n; /* start off Prim's with a random starting vertex */
    long row_size = (long) my_n/side;
	
	/* generate my portion of maze */
	while (i < my_n -1)
	{
		// /* DEBUGGING: PRINTING OUT VISITED ARRAY */
		// for (int j=0; j<cursor; j++)
		// 	fprintf(stderr, "%d ", my_maze[j]);
		// fprintf(stderr, "\n");

		/* find that random edge from visited vertices */
		/* store visited vertices in my_maze and keep track of a cursor */
		a = my_maze[rand() % cursor];
		rand_edge(my_n, (long) side, row_size, &a, &b);

		/* check if we have an already-visited edge */
		if (visited(b, cursor, my_maze) || b >= my_n)
			continue;

		/* generate pair with appropriate x and y values for push_back() */
		pair.x = a + comp;
		pair.y = b + comp;

		/* add edge to the array of edges and increment if successful */
		pthread_mutex_lock(&mutex);
		if (!push_back(pair)) {
			my_maze[cursor] = b;
			i++;
			cursor++;
		} else {
			fprintf(stderr, "error adding edge where there should be no error.\nexiting...\n");
			exit(1);
		}
		pthread_mutex_unlock(&mutex);
	}

    /* using tree-structured reduction */
    int r;
    int iter = 0;
    while (1)
    {
        if ((rank % (int) pow(2,iter+1)) == 0) {
            if (rank == 0 && (int) pow(2,iter) == thread_count) {
                /* maze is fully generated, exit process */
                pthread_exit(0);
            }

            /* wait for thread to complete its maze generation */
            pthread_join(thread_handles[(int) (rank+pow(2,iter))], NULL);

            /* resolve appropriate connection between 2 parts of maze */
            r = rand() % side;
            a = comp + my_n - side + r;
            b = comp + my_n + r;
            pair.x = a;
            pair.y = b;
			/* add edge to the array of edges */
			pthread_mutex_lock(&mutex);
			if (push_back(pair))
			{
				fprintf(stderr, "error adding edge in reduction.\nexiting...\n");
				exit(1);
			}
			pthread_mutex_unlock(&mutex);
			my_n = 2*my_n; /* adjusting size to accomodate for absorbed vertices */
        } else {
            pthread_exit(0);
        }
        iter++;
    }
}

void *threadSolve(void *my_rank)
{
	// long buf_size = 32;
	// long rank = (long) my_rank;
	// double start_time, end_time;
	// char buf[buf_size];

	// start_time = getProcessTime();

	// if (rank == 0)
	// {
	// 	/* random mouse algorithm */
	// 	sprintf(buf, "random mouse", buf_size);
	// } else if (rank == 1)
	// {
	// 	/* breadth-first search */
	// 	sprintf(buf, "random mouse", buf_size);
	// } else if (rank == 2)
	// {
	// 	 depth-first search 
	// 	buf = "depth-first search"
	// } else if (rank == 3)
	// {
	// 	/* right-hand solution */
	// 	buf = "right-hand"
	// } else
	// {
	// 	/* error message */
	// 	fprintf(stderr, "Incorrect number of threads passed into threadSolve() function.\nexiting...\n");
	// 	exit(0);
	// }

	// end_time = getProcessTime();
	// printf("Time to complete %s algorithm: %f", buf, end_time-start_time);
}

void generateMaze(MAZE* m, long size, long num_threads)
{
	/* add code to create pthreads */
    long thread;
    long i;
    double start_time, end_time;
    thread_count = num_threads;
    cursor = 0;
    thread_handles = malloc(num_threads*sizeof(pthread_t));
    pthread_mutex_init(&mutex, NULL);
    srand(time(0));

    /* make maze m global variable and initialize the size, vertices, and edge variables */
    gen_maze = m;
    m->side_length = size;
    m->edges = malloc((pow(size,2)-1)*sizeof(PAIR));
    m->vertices = malloc(pow(size,2)*sizeof(int));

    start_time = getWorldTime();
    /* Start the threads. */
    for (thread = 0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       threadGenerate, (void*) thread);

    /* Wait for thread 0 to finish, since thread 0 ends
    *  after all the other threads have completed */
    pthread_join(thread_handles[0], NULL);

    end_time = getWorldTime();

    printf("Time to generate maze: %f\n", end_time-start_time);
}

void solveMaze(MAZE* m)
{
	// /* add code to create pthreads */
 //    long       thread;
 //    pthread_t* thread_handles;
 //    long i;
 //    thread_handles = malloc(num_threads*sizeof(pthread_t));

 //    /* make maze m global variable and initialize the size*/
 //    gen_maze = m;
 //    (*maze)->side_length = size;

 //     Start the threads. 
 //    for (thread = 0; thread < threads; thread++)
 //        pthread_create(&thread_handles[thread], NULL,
 //                       threadGenerate, (void*) thread);

 //    /* Wait for threads to finish. */
 //    for (thread = 0; thread < threads; thread++)
 //        pthread_join(thread_handles[thread], NULL);
}

void printMaze(MAZE* m)
{
	/* add code to print maze contents */
	int i;
	long size = (long) pow(m->side_length,2)-1;
	PAIR *edge = m->edges;
	for (i=0; i<size; i++)
		printf("%d --- %d\n", edge[i].x, edge[i].y);
}