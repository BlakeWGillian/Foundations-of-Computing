/*
    Skeleton written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Implementation details for module which contains doubly-linked list 
        specification data structures and functions.
    
    Implemented by <YOU>
*/

#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>




/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingForward(list_t *list){
    /* IMPLEMENT HERE */
    if (list == NULL || list->head == NULL || list->tail == NULL) return;
    node_t* index = list->head;
    while (index != list->tail){ //index != tail
        printf("(%Lf, %Lf) ", index->x, index->y);
        index = index->tail;
    }
    printf("(%Lf, %Lf) ", list->tail->x, list->tail->y);
}

/* Print out first the first item of the list, then print out each value in 
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingBackwards(list_t *list){
    /* IMPLEMENT HERE */
    if (list == NULL || list->head == NULL || list->tail == NULL) return;
    node_t* index = list->tail;
    printf("(%Lf, %Lf) ", list->head->x, list->head->y);
    while (index != list->head){ //index != tail
        printf("(%Lf, %Lf) ", index->x, index->y);
        index = index->head;
    }
}

node_t* newNode(long double x, long double y){
    node_t* init = malloc(sizeof(node_t));
    init->head = NULL;
    init->tail = NULL;
    init->x = x;
    init->y = y;
    return init;
}

/* Return a new empty list. */
list_t* newList(){ 
    /* IMPLEMENT HERE */
    list_t *list = malloc(sizeof(list_t));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

/* Insert the given x, y pair at the head of the list */
void insertHead(list_t *list, long double x, long double y){
  /* IMPLEMENT HERE */
  node_t* new_head = newNode(x, y);
  if (list->head){
    new_head->tail = list->head;
    list->head->head = new_head;
  }
  if (list->tail == NULL){
    list->tail = new_head;
  }
  list->head = new_head;
}

/* Insert the given x, y pair at the tail of the list */
void insertTail(list_t *list, long double x, long double y){
  /* IMPLEMENT HERE */
  node_t* new_tail = newNode(x, y);
  if (list->tail){
    new_tail->head = list->tail;
    list->tail->tail = new_tail;
  }
  if (list->head == NULL){
    list->head = new_tail;
  }
  list->tail = new_tail;
}

void rec_freeList(node_t *head){
    if (head != NULL){
        rec_freeList(head->tail);
        free(head);
    }
}

/* Free all items in the given list. */
void freeList(list_t *list){
    rec_freeList(list->head);
}

