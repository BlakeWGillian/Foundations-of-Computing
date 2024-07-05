/*
graph.c

Set of vertices and edges implementation.

Implementations for helper functions for graph construction and manipulation.

Skeleton written by Grady Fitzpatrick for COMP20007 Assignment 1 2024
*/
#include "graph.h"
#include "pq.h"
#include "utils.h"
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>

#define INITIALEDGES 32
#define UNVISITED -1

struct edge;
void matrix_print(int **matrix, struct graph *g);

/* Definition of a graph. */
struct graph {
  int numVertices;
  int numEdges;
  int allocedEdges;
  struct edge **edgeList;
};

/* Definition of an edge. */
struct edge {
  int start;
  int end;
  int cost;
};

struct graph *newGraph(int numVertices) {
  struct graph *g = (struct graph *)malloc(sizeof(struct graph));
  assert(g);
  /* Initialise edges. */
  g->numVertices = numVertices;
  g->numEdges = 0;
  g->allocedEdges = 0;
  g->edgeList = NULL;
  return g;
}

/* Adds an edge to the given graph. */
void addEdge(struct graph *g, int start, int end, int cost) {
  assert(g);
  struct edge *newEdge = NULL;
  /* Check we have enough space for the new edge. */
  if ((g->numEdges + 1) > g->allocedEdges) {
    if (g->allocedEdges == 0) {
      g->allocedEdges = INITIALEDGES;
    } else {
      (g->allocedEdges) *= 2;
    }
    g->edgeList = (struct edge **)realloc(g->edgeList, sizeof(struct edge *) *
                                                           g->allocedEdges);
    assert(g->edgeList);
  }

  /* Create the edge */
  newEdge = (struct edge *)malloc(sizeof(struct edge));
  assert(newEdge);
  newEdge->start = start;
  newEdge->end = end;
  newEdge->cost = cost;

  /* Add the edge to the list of edges. */
  g->edgeList[g->numEdges] = newEdge;
  (g->numEdges)++;
}

/* Returns a new graph which is a deep copy of the given graph (which must be
  freed with freeGraph when no longer used). */
struct graph *duplicateGraph(struct graph *g) {
  struct graph *copyGraph = (struct graph *)malloc(sizeof(struct graph));
  assert(copyGraph);
  copyGraph->numVertices = g->numVertices;
  copyGraph->numEdges = g->numEdges;
  copyGraph->allocedEdges = g->allocedEdges;
  copyGraph->edgeList =
      (struct edge **)malloc(sizeof(struct edge *) * g->allocedEdges);
  assert(copyGraph->edgeList || copyGraph->numEdges == 0);
  int i;
  /* Copy edge list. */
  for (i = 0; i < g->numEdges; i++) {
    struct edge *newEdge = (struct edge *)malloc(sizeof(struct edge));
    assert(newEdge);
    newEdge->start = (g->edgeList)[i]->start;
    newEdge->end = (g->edgeList)[i]->end;
    newEdge->cost = (g->edgeList)[i]->cost;
    (copyGraph->edgeList)[i] = newEdge;
  }
  return copyGraph;
}

/* Frees all memory used by graph. */
void freeGraph(struct graph *g) {
  int i;
  for (i = 0; i < g->numEdges; i++) {
    free((g->edgeList)[i]);
  }
  if (g->edgeList) {
    free(g->edgeList);
  }
  free(g);
}

/* Returns the smallest integer, or the non-negative one*/
int d_min(int a, int b) {
  if (a < 0) {
    return b;
  } else if (b < 0) { // not really needed
    return a;
  }
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

/* Returns the smallest long double, or the non-negative one*/
long double lf_min(long double a, long double b) {
  if (a < 0) {
    return b;
  } else if (b < 0) { // not really needed
    return a;
  }
  if (a < b) {
    return a;
  } else {
    return b;
  }
}

// PROBLEM A
/* make an integer distance matrix, from the graph*/
int **distance_matrix(int numLocations, struct graph *g) {
  int **matrix = (int **)malloc(g->numVertices * sizeof(int *));
  for (int i = 0; i < g->numVertices; i++) {
    // make it a 2d array
    matrix[i] = (int *)malloc(g->numVertices * sizeof(int));
  }
  for (int i = 0; i < g->numVertices; i++) {
    for (int j = 0; j < g->numVertices; j++) {
      matrix[i][j] = UNVISITED;
    }
  }
  // add each edge to the distance matrix
  for (int i = 0; i < g->numEdges; i++) {
    matrix[((g->edgeList)[i])->start][((g->edgeList)[i])->end] =
        ((g->edgeList)[i])->cost;
    matrix[((g->edgeList)[i])->end][((g->edgeList)[i])->start] =
        ((g->edgeList)[i])->cost;
  }
  return matrix;
}

// Same implementation but with long doubles.
long double **distance_matrix_lf(int numLocations, struct graph *g) {
  long double **matrix =
      (long double **)malloc(g->numVertices * sizeof(long double *));
  for (int i = 0; i < g->numVertices; i++) {
    // 2d array
    matrix[i] = (long double *)malloc(g->numVertices * sizeof(long double));
  }
  for (int i = 0; i < g->numVertices; i++) {
    for (int j = 0; j < g->numVertices; j++) {
      matrix[i][j] = UNVISITED;
    }
  }
  // add each edge to the distance matrix
  for (int i = 0; i < g->numEdges; i++) {
    matrix[((g->edgeList)[i])->start][((g->edgeList)[i])->end] =
        (((g->edgeList)[i])->cost + 100) * 0.01;
    matrix[((g->edgeList)[i])->end][((g->edgeList)[i])->start] =
        (((g->edgeList)[i])->cost + 100) * 0.01;
  }
  return matrix;
}

int **matrix_copy(int **matrix, struct graph *g) {
  int **matrix2 = malloc(g->numVertices * sizeof(int *));
  for (int i = 0; i < g->numVertices; i++) {
    // 2d array
    matrix2[i] = malloc(g->numVertices * sizeof(int));
  }
  // add each edge to the distance matrix
  for (int i = 0; i < g->numVertices; i++) {
    for (int j = 0; j < g->numVertices; j++) {
      matrix2[i][j] = matrix[i][j];
    }
  }
  return matrix2;
}

long double **matrix_copy_lf(long double **matrix, struct graph *g) {
  long double **matrix2 = malloc(g->numVertices * sizeof(long double *));
  for (int i = 0; i < g->numVertices; i++) {
    // 2d array
    matrix2[i] = malloc(g->numVertices * sizeof(long double));
  }
  // add each edge to the distance matrix
  for (int i = 0; i < g->numVertices; i++) {
    for (int j = 0; j < g->numVertices; j++) {
      matrix2[i][j] = matrix[i][j];
    }
  }
  return matrix2;
}

// A *= B
int matrix_mult(int **A, int **B, enum problemPart part, struct graph *g) {
  int **A_o = matrix_copy(A, g);
  for (int i = 0; i < g->numVertices; i++) {
    for (int j = 0; j < g->numVertices; j++) {
      for (int q = 0; q < g->numVertices; q++) { // Index for row and col in AxB
        if (part == PART_A) {
          // If A (original) is visited, then A is visited.
          // If there is a link between A and any B, then A is visited.
          // Else, A is unvisisted
          A[i][j] = ((A_o[i][q] >= 0 && B[q][j] >= 0) || A[i][j] >= 0)
              UNVISITED; // since -1 defn
        } else if (part == PART_B) {
          if (A_o[i][q] >= 0 && B[q][j] >= 0) {
            A[i][j] = d_min(A[i][j], (A_o[i][q] + B[q][j]));
          }
        }
      }
    }
  }
  free(A_o);
  return 0;
}

// For PartD, multiply a distance matrix with long doubles. A *= B
int matrix_mult_lf(long double **A, long double **B, enum problemPart part,
                   struct graph *g) {
  // Create a copy so the original matrix doesn't change when being multiplied
  long double **A_o = matrix_copy_lf(A, g);
  for (int i = 0; i < g->numVertices; i++) {
    for (int j = 0; j < g->numVertices; j++) {
      for (int q = 0; q < g->numVertices; q++) { // Index for row and col in AxB
        if (A_o[i][q] >= 0 && B[q][j] >= 0) {
          if (A[i][j] < 0) {
            A[i][j] = A_o[i][q] * B[q][j];
          } else {
            A[i][j] = lf_min(A[i][j], A_o[i][q] * B[q][j]);
          }
        }
      }
    }
  }
  free(A_o);
  return 0;
}

// Mark a node as visited - get rid of all associated edges
int mark_visited(struct graph *g, int end) {
  for (int i = 0; i < g->numEdges; i++) {
    // If this node is at the end, get rid of it
    if ((g->edgeList)[i]->end == end) {
      (g->edgeList)[i]->end = UNVISITED;
      // If this node is at the start, get rid of it
    } else if ((g->edgeList)[i]->start == end) {
      (g->edgeList)[i]->start = UNVISITED;
    }
  }
  return 0;
}

// Makes the cheapest move, and returns its cost.
// Also removes the node moved to from the graph.
int cheapest_move(struct graph *g) {
  int cost = UNVISITED;
  int best = UNVISITED;
  for (int i = 0; i < g->numEdges; i++) {
    if (((g->edgeList)[i]->end == UNVISITED) !=
        ((g->edgeList)[i]->start == UNVISITED)) {
      if (d_min(cost, (g->edgeList)[i]->cost) != cost) {
        best = i;
        cost = (g->edgeList)[i]->cost;
      }
    }
  }
  if (best != UNVISITED) {
    mark_visited(g, (g->edgeList[best])->start);
    mark_visited(g, (g->edgeList[best])->end);
    return cost;
  }
  return 0;
}

// Some functions for debugging
/*int print_edges(struct graph *g){
  for(int i = 0; i < g->numEdges; i++){
    printf("%d to %d c:%d, ", (g->edgeList)[i]->start, (g->edgeList)[i]->end,
(g->edgeList)[i]->cost);
  }
  printf("\n");
  return 0;
}

void matrix_print(int** matrix, struct graph *g){
  printf("MATRIX:\n");
  for (int i = 0; i < g->numVertices; i++){
    for (int j = 0; j < g->numVertices; j++){
      printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}

void matrix_print_lf(long double** matrix, struct graph *g){
  printf("MATRIX:\n");
  for (int i = 0; i < g->numVertices; i++){
    for (int j = 0; j < g->numVertices; j++){
      printf("%Lf ", matrix[i][j]);
    }
    printf("\n");
  }
}*/

struct solution *graphSolve(struct graph *g, enum problemPart part,
                            int numLocations, int startingLocation,
                            int finalLocation) {
  struct solution *solution =
      (struct solution *)malloc(sizeof(struct solution));

  assert(solution);
  if (part == PART_A) {
    // Put the problem into a distance matrix.
    int **matrix = distance_matrix(numLocations, g);
    int **matrix_o = matrix_copy(matrix, g);
    int steps = 1;

    // If a walk is not found from start to finish, multiply.
    // Or, until the multiplications is higher than nodes
    // (i.e. the maximum number of steps is never higher than nodes)
    while (matrix[startingLocation][finalLocation] == UNVISITED &&
           steps <= g->numVertices) {
      steps++;
      matrix_mult(matrix, matrix_o, part, g);
    }
    free(matrix_o);
    free(matrix);
    solution->damageTaken = steps;
  } else if (part == PART_B) {
    // Put the problem into a distance matrix.
    int **matrix = distance_matrix(numLocations, g);
    int **matrix_o = matrix_copy(matrix, g);
    /* Since the cost of a path is never below 0, the most efficient one
      * does not visit a node more than once. So, it can be found with
       numVerticies multiplications */
    for (int i = 0; i < g->numVertices; i++) {
      matrix_mult(matrix, matrix_o, part, g);
    }
    // The minimum cost is stored by the distance matrix
    solution->totalCost = matrix[startingLocation][finalLocation];
    free(matrix_o);
    free(matrix);
  } else if (part == PART_C) {
    int cost = 0;
    // Do not move back to 0
    mark_visited(g, 0);

    // using dijkstra's algorithm mark each verticy with the shortest path.
    for (int i = 0; i < g->numVertices; i++) {
      cost += cheapest_move(g);
    }
    solution->artisanCost = cost;
  } else { // PART_D
    /* Make the problem into a distance matrix, but with 100% added.
      this is so elements can be multiplied to find the damage of a walk*/
    long double **matrixd = distance_matrix_lf(numLocations, g);
    long double **matrix_od = matrix_copy_lf(matrixd, g);

    // Using a slightly modified distance matrix, find the shortest path
    // (but with cost being multiplied not added)
    for (int i = 0; i < g->numVertices; i++) {
      matrix_mult_lf(matrixd, matrix_od, part, g);
    }
    // Minus 100%, since it is the additional damage not total being returned
    solution->totalPercentage =
        (matrixd[startingLocation][finalLocation] * 100) - 100;
    free(matrix_od);
    free(matrixd);
  }
  return solution;
}
