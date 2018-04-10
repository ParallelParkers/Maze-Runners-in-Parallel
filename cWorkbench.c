#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* function declarations */
void reset(int number_of_sets);
int Find(int element);
int GetNumberOfSets();

typedef enum { false, true } bool;

int *sets;
int number_of_sets = 0;

void Reset(int number_of_sets) {
    memset(sets, 0, sizeof(int)*number_of_sets);
    sets = realloc(sets, (number_of_sets -1)*sizeof(int));
    //)number_of_sets = number_of_sets; // Not sure what this does
}

bool Union(int set_a, int set_b) {
    int root_a = Find(set_a);
    int root_b = Find(set_b);
    if (root_a == root_b)
        return false;
    sets[root_a] += sets[root_b];
    sets[root_b] = root_a;
    number_of_sets--;
    return true;
}

int Find(int element) {
    if (sets[element] < 0)
        return element;
    return (sets[element] = Find(sets[element]));
}

int GetNumberOfSets() {
    return number_of_sets;
}



int main() {
  srand(time(0));
//  std::vector<std::pair<int, int> > all_edges;
//  std::vector<std::pair<int, int> > mst_edges;
//  std::vector<std::pair<int, int> > points;
//  CreateEdges(&all_edges);
//  ShuffleVector(&all_edges);
//  Kruskal(all_edges, &mst_edges);
//  ConvertToPaintedPoint(mst_edges, &points);
//  Draw(points);
  return 0;
}
