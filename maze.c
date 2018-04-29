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
	PAIR pair, *my_edges;
	int i, a, b, side, comp, cursor, *my_maze;
	i = 0;
	cursor = 1;
	side = gen_maze->side_length;
	rank = (long) my_rank;
	n = (long) pow(side,2);
	my_n = n/thread_count;
	comp = (int) my_n*rank;
	my_edges = (PAIR*)malloc((my_n-1)*sizeof(PAIR));
	my_maze = &((gen_maze->vertices)[comp]);
	my_maze[0] = rand() % my_n; /* start off Prim's with a random starting vertex */
    long row_size = (long) my_n/side;
	
	/* generate my portion of maze */
	while (i < my_n -1)
	{
		/* find random edge from visited vertices */
		/* store visited vertices in my_maze and keep track of a cursor */
		a = my_maze[rand() % cursor];
		rand_edge(my_n, (long) side, row_size, &a, &b);

		/* PROPOSED OPTIMIZATION
		if b equals -1:
			// could implement using heapsort to simulate dynamic sizing
			pop a out of my_maze so it is not visited again
			cursor--
			continue
		*/

		/* check if we have an already-visited edge */
		if (visited(b, cursor, my_maze) || b >= my_n || b == -1)
			continue;

		/* generate pair with appropriate x and y values */
		pair.x = a + comp;
		pair.y = b + comp;

		/* add edge to the array of edges and increment if successful */
		my_edges[cursor-1]=pair;
		my_maze[cursor] = b;
		i++;
		cursor++;
	}

	/* add local edges to the global edges array */ 
	pthread_mutex_lock(&mutex);
	for(i=0; i<my_n-1; i++)
		push_back(my_edges[i]);
	pthread_mutex_unlock(&mutex);

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

void printMaze(MAZE* m)
{
	/* add code to print maze contents */
	int i;
	long size = (long) pow(m->side_length,2)-1;
	PAIR *edge = m->edges;
	for (i=0; i<size; i++)
		printf("%d --- %d\n", edge[i].x, edge[i].y);
}