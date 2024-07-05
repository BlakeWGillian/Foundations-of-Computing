/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 1 2024 Semester 1
    
    Header for module which contains doubly-linked list 
        specification data structures and functions.
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H


typedef struct linkedListNode node_t;
typedef struct linkedList list_t;

struct linkedListNode {
    /* IMPLEMENT HERE */
    node_t* head; //parent
    node_t* tail; //child

    long double x;
    long double y;
};

struct linkedList {
    node_t *head;
    node_t *tail;
};



/* Print out each value in the list from the first value to the final value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingForward(list_t *list);

/* Print out first the first item of the list, then print out each value in 
    the list from the final value to the second value.
    Each value is printed with the format "(x, y) " where x is the x value
    set when inserting the item and y is the y value set when inserting 
    the item. */
void traverseRingBackwards(list_t *list);

/* Return a new empty list. */
list_t *newList();

/* Insert the given x, y pair at the head of the list */
void insertHead(list_t *list, long double x, long double y);

/* Insert the given x, y pair at the tail of the list */
void insertTail(list_t *list, long double x, long double y);

/* Free all items in the given list. */
void freeList(list_t *list);

#endif
