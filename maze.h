#ifndef MAZE_H
#define MAZE_H

/* NOTE: this is a prototype structure; adjust however it is necessary */
typedef struct
{
	int x;
	int y;
} PAIR;

typedef struct {
	long side_length; /* side length of maze (which is always square) */
	int* vertices;   /* 2d array which can be used to store visited squares */
	PAIR *edges;      /* 2d array which stores connections -- i.e. edges[1][2] == 1 means node 1 points to node 2 (and should be symmetrical) */
} MAZE;

/* function declarations */
void generateMaze(MAZE* maze, long size, long num_threads);
void solveMaze(MAZE* maze);

#endif