/*
    Skeleton written by Grady Fitzpatrick for
    COMP20007 Assignment 1 2024 Semester 1

    Header for module which contains convex hull
        specification data structures and functions.

    Implemented by <ME>
*/
#include "convexHull.h"
#include "linkedList.h"
#include "problem.h"
#include "stack.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

enum orientationResult { COLLINEAR = 0, CLOCKWISE = 1, COUNTERCLOCKWISE = 2 };

/* Finds the orientation between the three given points - calculates the angle
   between the Middle-First vector and Middle-Final vector - if the Middle-First
   vector is zero-length, the gradient of the Middle-Final vector is instead
   used. */
enum orientationResult orientation(struct problem *p, int idxFirst,
                                   int idxMiddle, int idxFinal);

enum orientationResult orientation(struct problem *p, int idxFirst,
                                   int idxMiddle, int idxFinal) {
  // assert(idxFirst >= 0 && idxFirst < p->numPoints);
  assert(idxMiddle >= 0 && idxMiddle < p->numPoints);
  assert(idxFinal >= 0 && idxFinal < p->numPoints);

  /* Use cross-product to calculate turn direction. */
  long double p0x = p->pointsX[idxFirst];
  long double p0y = p->pointsY[idxFirst];

  long double p1x = p->pointsX[idxMiddle];
  long double p1y = p->pointsY[idxMiddle];

  long double p2x = p->pointsX[idxFinal];
  long double p2y = p->pointsY[idxFinal];

  /* Cross product of vectors P1P0 & P1P2 */
  long double crossProduct =
      (p0x - p1x) * (p2y - p1y) - (p0y - p1y) * (p2x - p1x);

  if (crossProduct == 0) {
    if (idxFirst == idxMiddle) {
      /* Special case where we are only looking for positive slope of P1P2. */
      if (p2x == p1x) {
        /* Special case: dx = 0, vertical */
        if (p2y < p1y) {
          /* Directly upwards */
          return COUNTERCLOCKWISE;
        } else if (p2y == p1y) {
          /* Same point. */
          return COLLINEAR;
        } else {
          return CLOCKWISE;
        }
      }
      long double m = (p2y - p1y) / (p2x - p1x);
      if (m >= 0) {
        return COUNTERCLOCKWISE;
      } else {
        return CLOCKWISE;
      }
    }
    return COLLINEAR;
  } else if (crossProduct > 0) {
    return CLOCKWISE;
  } else {
    return COUNTERCLOCKWISE;
  }
}

/* Finds the leftmost (or equal leftmost, lowest)
 * point in a problem and returns its index. */
int leftmost_point(struct problem *p) {
  int lft_index = 0;

  // For each point in the problem, check if it is lowest
  for (int i = 0; i < p->numPoints; i++) {
    if ((p->pointsX)[lft_index] > (p->pointsX)[i]) {
      lft_index = i;

      // Secondary sort by X position
    } else if (((p->pointsX)[lft_index] == (p->pointsX)[i]) &&
               (p->pointsY)[lft_index] > (p->pointsY)[i])
      lft_index = i;
  }
  return lft_index;
}

/* Finds the lowest (or equal lowest, leftmost)
 * point in a problem and returns its index. */
int lowest_point(struct problem *p) {
  int lft_index = 0;

  // For each point in the problem
  for (int i = 0; i < p->numPoints; i++) {
    if ((p->pointsY)[lft_index] > (p->pointsY)[i]) {
      lft_index = i;

      // Secondary sort by Y position
    } else if (((p->pointsY)[lft_index] == (p->pointsY)[i]) &&
               (p->pointsX)[lft_index] > (p->pointsX)[i])
      lft_index = i;
  }
  return lft_index;
}

/* Returns the polar coordinates in radians from
 * element a to b, in a problem. */
long double polar(struct problem *p, int a, int b) {
  long double p1x = p->pointsX[b] - p->pointsX[a];
  long double p1y = p->pointsY[b] - p->pointsY[a];

  // Arc tangent of x/y, for angle in radians from positive x axis.
  long double radians = atan2(p1y, p1x);
  return radians;
}

/* Swaps two points in a problem, given their indices */
void swap_points(struct problem *p, int a, int b) {
  long double p1x = (p->pointsX)[a];
  long double p1y = (p->pointsY)[a];

  (p->pointsX)[a] = (p->pointsX)[b];
  (p->pointsY)[a] = (p->pointsY)[b];

  (p->pointsX)[b] = p1x;
  (p->pointsY)[b] = p1y;
}

/* Returns the index of the closest point from the origin (o) */
int closest_point(struct problem *p, int a, int b, int o) {
  long double xA = (p->pointsX)[a] - (p->pointsX)[o];
  long double yA = (p->pointsY)[a] - (p->pointsY)[o];
  long double xB = (p->pointsX)[b] - (p->pointsX)[o];
  long double yB = (p->pointsY)[b] - (p->pointsY)[o];

  // x^2 + y^2 = (distance)^2
  long double A = xA * xA + yA * yA;
  long double B = xB * xB + yB * yB;

  // Since (distance)^2 is monotone, sqrt() is not needed.
  if (A <= B)
    return A;
  else
    return B;
}

/* Sorts points according to the pivot (hi) and returns the
 * new pivot's index */
int partition(struct problem *p, int lo, int hi, int o, int *ops) {
  long double pivot = polar(p, o, hi);
  int k = lo - 1;

  // Sort the elements accoring to pivot from lo to hi
  for (int j = lo; j < hi; j++) {
    if (polar(p, o, j) < pivot) {
      k++;
      swap_points(p, k, j);
    }
    *ops += 1;
  }
  swap_points(p, k + 1, hi);
  return (k + 1);
}

/* Implementation of quick sort in n * log(n) average time.
 * ops is a pointer to the operations counter */
void quicksort(struct problem *p, int lo, int hi, int o, int *ops) {
  if (lo < hi) {
    int part = partition(p, lo, hi, o, ops);
    quicksort(p, lo, part - 1, o, ops);
    quicksort(p, part + 1, hi, o, ops);
  }
}

struct solution *jarvisMarch(struct problem *p) {
  /* Part A - perform Jarvis' March to construct a convex
  hull for the given problem. */
  struct solution *s = (struct solution *)malloc(sizeof(struct solution));
  assert(s);
  s->operationCount = 0;
  if (p->numPoints < 3) {
    s->convexHull = NULL;
    return s;
  }
  list_t *hull = newList();

  int leftmost = leftmost_point(p);
  int current = leftmost;
  int nextPoint;

  do {
    insertTail(hull, (p->pointsX)[current], (p->pointsY)[current]);

    nextPoint = 0;
    for (int point = 0; point < p->numPoints; point++) {
      if (nextPoint == current) {
        nextPoint = point;
      } else if (orientation(p, nextPoint, current, point) ==
                 COUNTERCLOCKWISE) {
        nextPoint = point;
        s->operationCount += 1;
        // If two points are collinear, the convex hull must first contain the
        // closest one.
      } else if (orientation(p, nextPoint, current, point) == COLLINEAR &&
                 point == closest_point(p, point, nextPoint, current)) {
        nextPoint = point;
        // s->operationCount += 1;
      }
    }

    current = nextPoint;
  } while (current != leftmost);
  // printf("\nOperations Jarvis: %d\n", s->operationCount);
  s->convexHull = hull;
  return s;
}

/* Pushes and integer onto the stack, and changed the buddy variable */
void s_push(int *stack, int a, int *len) {
  stack[*len] = a;
  *len += 1;
}

struct solution *grahamScan(struct problem *p) {
  /* Part B - perform Graham's Scan to construct a convex
  hull for the given problem. */
  /* IMPLEMENT HERE */
  struct solution *s = (struct solution *)malloc(sizeof(struct solution));
  assert(s);
  s->operationCount = 0;
  if (p->numPoints < 3) {
    s->convexHull = NULL;
    return s;
  }
  list_t *hull = newList();

  int lowest = lowest_point(p);
  // Remove the origin so it's not sorted
  swap_points(p, lowest, 0);
  p->pointsX += 1;
  p->pointsY += 1;
  p->numPoints -= 1;
  lowest = -1; // Now before the 0th element of the array

  // Sort the points
  quicksort(p, 0, p->numPoints - 1, lowest, &s->operationCount);

  // Restore the origin to the points list
  p->pointsX -= 1;
  p->pointsY -= 1;
  p->numPoints += 1;

  // Initialize and push 3
  int *stack = malloc(sizeof(int) * p->numPoints);
  int stack_len = 0;
  s_push(stack, 0, &stack_len);
  s_push(stack, 1, &stack_len);
  s_push(stack, 2, &stack_len);

  for (int i = 3; i < p->numPoints; i++) {
    while (orientation(p, stack[stack_len - 2], stack[stack_len - 1], i) !=
           COUNTERCLOCKWISE) {
      stack_len -= 1;
    }
    s_push(stack, i, &stack_len);
  }

  for (int i = 0; i < stack_len; i++) {
    insertTail(hull, p->pointsX[stack[i]], p->pointsY[stack[i]]);
  }

  // printf("\nOperations Graham: %d\n", s->operationCount);
  s->convexHull = hull;
  free(stack);
  return s;
}

void freeSolution(struct solution *s) {
  if (!s) {
    return;
  }
  if (s->convexHull) {
    freeList(s->convexHull);
  }
  free(s);
}
