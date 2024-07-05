/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
    Implementation for module which contains  
        Problem 2-related data structures and 
        functions.
    
    Sample solution implemented by Grady Fitzpatrick
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include "problem.h"
#include "problemStruct.c"
#include "solutionStruct.c"
#include "prefixTree.h"

/* Number of words to allocate space for initially. */
#define INITIALWORDSALLOCATION 64

#define VISITED 'X'

/* Denotes that the dimension has not yet been set. */
#define DIMENSION_UNSET (-1)

struct problem;
struct solution;

/* Sets up a solution for the given problem. */
struct solution *newSolution(struct problem *problem);

/* Compares the value of two c strings
 * -1 if a < b, 0 if a = b, 1 a > b
 * */
int compare_strings(const void *a, const void *b){
  return strcmp(*(char **)a, *(char **)b);
}

/* 
    Reads the given dict file into a list of words 
    and the given board file into a nxn board.
*/
struct problem *readProblemA(FILE *dictFile, FILE *boardFile){
    struct problem *p = (struct problem *) malloc(sizeof(struct problem));
    assert(p);

    /* Part B onwards so set as empty. */
    p->partialString = NULL;

    int wordCount = 0;
    int wordAllocated = 0;
    char *dictText = NULL;
    char **words = NULL;

    /* Read in text. */
    size_t allocated = 0;
    /* Exit if we read no characters or an error caught. */
    int success = getdelim(&dictText, &allocated, '\0', dictFile);

    if(success == -1){
        /* Encountered an error. */
        perror("Encountered error reading dictionary file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    char *boardText = NULL;
    /* Reset allocated marker. */
    allocated = 0;
    success = getdelim(&boardText, &allocated, '\0', boardFile);

    if(success == -1){
        /* Encountered an error. */
        perror("Encountered error reading board file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    /* Progress through string. */
    int progress = 0;
    /* Table string length. */
    int dictTextLength = strlen(dictText);

    /* Count words present. */
    while(progress < dictTextLength){
        char *word = NULL;
        int nextProgress;
        /* Read each value into the dictionary. */
        if(progress == 0){
            /* First line. */
            int wordNumberGuess;
            assert(sscanf(dictText + progress, "%d %n", &wordNumberGuess, &nextProgress) == 1);
            /* Will fail if integer missing from the start of the words. */
            assert(nextProgress > 0);
            if(wordNumberGuess > 0){
                wordAllocated = wordNumberGuess;
                words = (char **) malloc(sizeof(char *) * wordAllocated);
                assert(words);
            }
            progress += nextProgress;
            continue;
        } else {
            assert(sscanf(dictText + progress, "%m[^\n] %n", &word, &nextProgress) == 1);
            assert(nextProgress > 0);
            progress += nextProgress;
        }

        /* Check if more space is needed to store the word. */
        if(wordAllocated <= 0){
            words = (char **) malloc(sizeof(char *) * INITIALWORDSALLOCATION);
            assert(words);
            wordAllocated = INITIALWORDSALLOCATION;
        } else if(wordCount >= wordAllocated){
            words = (char **) realloc(words, sizeof(char *) * 
                wordAllocated * 2);
            assert(words);
            wordAllocated = wordAllocated * 2;
            /* Something has gone wrong if there's not sufficient word 
                space for another word. */
            assert(wordAllocated > wordCount);
        }

        words[wordCount] = word;
        wordCount++;
    }

    
    qsort(words,  wordCount, sizeof(char*), compare_strings);
    
    /* Done with dictText */
    if(dictText){
        free(dictText);
    }
    
    /* Now read in board */
    progress = 0;
    int dimension = 0;
    int boardTextLength = strlen(boardText);
    /* Count dimension with first line */
    while(progress < boardTextLength){
        /* Count how many non-space characters appear in line. */
        if(boardText[progress] == '\n' || boardText[progress] == '\0'){
            /* Reached end of line. */
            break;
        }
        if(isalpha(boardText[progress])){
            dimension++;
        }
        progress++;
    }

    assert(dimension > 0);

    /* Check each line has the correct dimension. */
    for(int i = 1; i < dimension; i++){
        int rowDim = 0;
        if(boardText[progress] == '\n'){
            progress++;
        }
        while(progress < boardTextLength){
            /* Count how many non-space characters appear in line. */
            if(boardText[progress] == '\n' || boardText[progress] == '\0'){
                /* Reached end of line. */
                break;
            }
            if(isalpha(boardText[progress])){
                rowDim++;
            }
            progress++;
        }
        if(rowDim != dimension){
            fprintf(stderr, "Row #%d had %d letters, different to Row #1's %d letters.\n", i + 1, rowDim, dimension);
            assert(rowDim == dimension);
        }
    }

    /* Define board. */
    char *boardFlat = (char *) malloc(sizeof(char) * dimension * dimension);
    assert(boardFlat);
    
    /* Reset progress. */
    progress = 0;
    for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
            int nextProgress;
            assert(sscanf(boardText + progress, "%c %n", &boardFlat[i * dimension + j], &nextProgress) == 1);
            progress += nextProgress;
        }
    }

    char **board = (char **) malloc(sizeof(char **) * dimension);
    assert(board);
    for(int i = 0; i < dimension; i++){
        board[i] = &boardFlat[i * dimension];
    }

    // fprintf(stderr, "\n");

    /* The number of words in the text. */
    p->wordCount = wordCount;
    /* The list of words in the dictionary. */
    p->words = words;

    /* The dimension of the board (number of rows) */
    p->dimension = dimension;

    /* The board, represented both as a 1-D list and a 2-D list */
    p->boardFlat = boardFlat;
    p->board = board;

    /* For Part B only. */
    p->partialString = NULL;

    p->part = PART_A;

    return p;
}

struct problem *readProblemB(FILE *dictFile, FILE *boardFile, 
    FILE *partialStringFile){
    /* Fill in Part A sections. */
    struct problem *p = readProblemA(dictFile, boardFile);

    char *partialString = NULL;

    /* Part B has a string that is partially given - we assume it follows 
        word conventions for the %s specifier. */
    assert(fscanf(partialStringFile, "%ms", &partialString) == 1);
    
    p->part = PART_B;
    p->partialString = partialString;

    return p;
}

struct problem *readProblemD(FILE *dictFile, FILE *boardFile){
    /* Interpretation of inputs is same as Part A. */
    struct problem *p = readProblemA(dictFile, boardFile);
    
    p->part = PART_D;
    return p;
}

/*
    Outputs the given solution to the given file. If colourMode is 1, the
    sentence in the problem is coloured with the given solution colours.
*/
void outputProblem(struct problem *problem, struct solution *solution, 
    FILE *outfileName){
    assert(solution);
    switch(problem->part){
        case PART_A:
        case PART_D:
            assert(solution->foundWordCount == 0 || solution->words);
            for(int i = 0; i < solution->foundWordCount; i++){
                fprintf(outfileName, "%s\n", solution->words[i]);
            }
            break;
        case PART_B:
            assert(solution->foundLetterCount == 0 || solution->followLetters);
            for(int i = 0; i < solution->foundLetterCount; i++){
                if(isalpha(solution->followLetters[i])){
                    fprintf(outfileName, "%c\n", solution->followLetters[i]);
                } else {
                    fprintf(outfileName, " \n");
                }
            }
            break;
    }
}

/*
    Frees the given solution and all memory allocated for it.
*/
void freeSolution(struct solution *solution, struct problem *problem){
    if(solution){
        if(solution->followLetters){
            free(solution->followLetters);
        }
        if(solution->words){
            free(solution->words);
        }
        free(solution);
    }
}

void freeProblem(struct problem *problem){
    if(problem){
        if(problem->words){
            for(int i = 0; i < problem->wordCount; i++){
                if(problem->words[i]){
                    free(problem->words[i]);
                }
            }
            free(problem->words);
        }
        if(problem->board){
            free(problem->board);
        }
        if(problem->boardFlat){
            free(problem->boardFlat);
        }
        if(problem->partialString){
            free(problem->partialString);
        }
        free(problem);
    }
}

/* Sets up a solution for the given problem */
struct solution *newSolution(struct problem *problem){
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->foundWordCount = 0;
    s->words = NULL;
    s->foundLetterCount = 0;
    s->followLetters = NULL;
    
    return s;
}



char **copy(char **board, int dims){
    char **dupe = malloc(sizeof(char *) * dims);
    for(int i = 0; i < dims; i++){
        dupe[i] = malloc(sizeof(char) * dims);
        for(int j = 0; j < dims; j++){
            dupe[i][j] = board[i][j];
        }
    }
    return dupe;
}

char **matrix(int a, int b){
    char **dupe = malloc(sizeof(char *) * a);
    for(int i = 0; i < a; i++){
        dupe[i] = malloc(sizeof(char) * b);
        for(int j = 0; j < b; j++){
            dupe[i][j] = '\0';
        }
    }
    return dupe;
}

char **lower(char **board, int dims){
    char **dupe = malloc(sizeof(char *) * dims);
    for(int i = 0; i < dims; i++){
        dupe[i] = malloc(sizeof(char) * dims);
        for(int j = 0; j < dims; j++){
            dupe[i][j] = tolower(board[i][j]);
        }
    }
    return dupe;
}

void visitDown(struct prefixTree *pt){
  if(pt == NULL) return;
  pt->vis += 1;
  visitDown(pt->parent);
}

int dMin(int a, int b){
  if(a < b) return a;
  return b;
}

int dMax(int a, int b){
  if(a > b) return a;
  return b;
}

void printBoard(char ** board, int dims){
  for (int i = 0; i < dims; i++) {
    for(int j = 0; j < dims; j++){
      printf("%c ", board[i][j]);
    }
    printf("\n");
  }
}

void markPrune(struct prefixTree *pt, char **board, int dims, int a, int b){
  board[a][b] = ' ';

  for(int k = 0; k < pt->noch; k++){
    if(pt->children[k]->value == '\0' && pt->children[k]->vis == 0){
      visitDown(pt->children[k]);
      continue;
    }
    for(int i = dMax(a - 1, 0); i < dMin(a + 2, dims); i++){
      for(int j = dMax(b - 1, 0); j < dMin(b + 2, dims); j++){
        if(board[i][j] == pt->children[k]->value){
          markPrune(pt->children[k], copy(board, dims), dims, i, j);
        }
      }
    }
  }
}

void initPrune(struct prefixTree *pt, char **board, int dims){
  for(int k = 0; k < pt->noch; k++){
    for(int i = 0; i < dims; i++){
      for(int j = 0; j < dims; j++){
        if(board[i][j] == pt->children[k]->value){
          markPrune(pt->children[k], copy(board, dims), dims, i, j);
        }
      }
    }
  }
}

void fillWords(char **m, struct prefixTree *pt, int depth){
  int sum = 0;
  for(int i = 0; i < pt->vis && depth >= 0; i++){
    m[i][depth] = pt->value;
  }
  for(int k = 0; k < pt->noch; k++){
    if(pt->children[k]->vis){
      fillWords(m + sum, pt->children[k], depth + 1);
      sum += pt->children[k]->vis;
    }
  }
}

int lowerWord(char *a, char *b){ // No word will be found twice.
  if(a[0] < b[0]) return 1;
  else if (a[0] > b[0]) return 0;
  return lowerWord(a + 1, b + 1);
}

void swapWord(char **m, int a, int b){
  char *tmp = m[b];
  m[b] = m[a];
  m[a] = tmp;
}

/*
    Solves the given problem according to Part A's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveProblemA(struct problem *p){
    struct solution *s = newSolution(p);
    /* Fill in: Part A */
    // Initiate the prefix tree with all words.
    struct prefixTree *pt = newPrefixTree();
    for(int i = 0; i < p->wordCount; i++){
        pt = addWordToTree(pt, p->words[i]); //CHECK
    }

  initPrune(pt, lower(p->board, p->dimension), p->dimension);
  char **m = matrix(pt->vis, 16);
  fillWords(m, pt, -1);

  s->words = m;
  s->foundWordCount = pt->vis;
    return s;
}


struct prefixTree *findMatch(struct prefixTree *pt, char *c){
  while(c[0] != '\0'){
    for(int k = 0; k < pt->noch; k++){
      if(pt->children[k]->value == c[0]){
        pt = pt->children[k];
        c += 1;
        goto jump;
      }
    }
    break;
  jump:;
  }
  if(c[0] != '\0') return NULL;
  pt->vis = 0;
  for(int k = 0; k < pt->noch; k++){
    if(pt->children[k]->vis) pt->vis++;
  }
  return pt;
}

struct solution *solveProblemB(struct problem *p){
  struct solution *s = newSolution(p);
  /* Fill in: Part B */
  //p->board = lower(p->board, p->dimension);
  

  // Initiate the prefix tree with all words.
  struct prefixTree *pt = newPrefixTree();
  for(int i = 0; i < p->wordCount; i++){
      pt = addWordToTree(pt, p->words[i]); //CHECK
  }

  initPrune(pt, lower(p->board, p->dimension), p->dimension);
  pt = findMatch(pt, p->partialString);

  s->followLetters = malloc(sizeof(char) * pt->vis);
  int i = -1;
  int j = 0;
  while(j < pt->vis){
    i++;
    if(pt->children[i]->vis == 0) break;
    s->followLetters[j] = pt->children[i]->value;
    j++;
  }
  s->foundLetterCount = pt->vis;
    return s;
}

int ***hashBoard(char **board, int dims){
  int ***m = malloc(sizeof(int ***) * (27 + 1));
  for(int i = 0; i < 27 + 1; i++){
    m[i] = malloc(sizeof(int**) * (27 + 1));
    for(int j = 0; j < 27 + 1; j++){
      m[i][j] = calloc(dims + 1, sizeof(int*));
      assert(m);
    }
  }

  for(int a = 0; a < dims; a++){
    for(int b = 0; b < dims; b++){
      // Now add the 8 possible moves to the table
      for(int i = dMax(a - 1, 0) - a; i < dMin(a + 2, dims) - a; i++){
        for(int j = dMax(b - 1, 0) - b; j < dMin(b + 2, dims) - b; j++){
          if(i == 0 && j == 0) continue;
          m[board[a][b] - 'a'][board[a + i][b + j] - 'a'][0] += 1;
          // If letter is the same then change
        }
      }
    }
  }
  return m;
}

int *copyCol(int *col){
  int *a = malloc(sizeof(int*) * 27 + 1);
  for(int i = 0; i < 27 + 1; i++){
    a[i] = col[i];
  } 
  return a;
}

int resolveDupe(struct prefixTree *pt, int ***hashBoard){
  printf("COLL\n");
  return 1; // Resolution
}

void markPruneD(struct prefixTree *pt, int ***hashBoard, int *collisions){
  collisions[pt->value - 'a'] = 1;

  for(int k = 0; k < pt->noch; k++){
    if(pt->children[k]->value == '\0' && pt->children[k]->vis == 0){// Last clause is redundant
      visitDown(pt->children[k]);
      continue;
    } 
    if(hashBoard[pt->value - 'a'][pt->children[k]->value - 'a'][0] &&
        collisions[pt->children[k]->value - 'a'] == 0){
        if(hashBoard[pt->value - 'a'][pt->children[k]->value - 'a'][0] - 1 > 0 && resolveDupe(pt->children[k], hashBoard) == 0){
          continue;
        }
        markPruneD(pt->children[k], hashBoard, copyCol(collisions));
    }
  }
}

void initPruneD(struct prefixTree *pt, int ***hashBoard, int *collisions){
  for(int k = 0; k < pt->noch; k++){
    if(pt->children[k]->value == '\0' && pt->children[k]->vis == 0){// Last clause is redundant
      visitDown(pt->children[k]);
      continue;
    } 
    if(collisions[pt->children[k]->value - 'a'] == 0){
      markPruneD(pt->children[k], hashBoard, copyCol(collisions));
    }
  }
}


struct solution *solveProblemD(struct problem *p){
    struct solution *s = newSolution(p);
    /* Fill in: Part A */
  p->board = lower(p->board, p->dimension);

struct prefixTree *pt = newPrefixTree();
  for(int i = 0; i < p->wordCount; i++){
      pt = addWordToTree(pt, p->words[i]); //CHECK
  }

    // Initiate the hash table to number of possible board positions.
  int ***hashedBoard = hashBoard(p->board, p->dimension);

  int *collisions = calloc(27 + 1, sizeof(int *));

  initPruneD(pt, hashedBoard, collisions);
  
  char **m = matrix(pt->vis, 16);
  fillWords(m, pt, -1);

  s->words = m;
  s->foundWordCount = pt->vis;
  return s;


  for(int i = 0; i < 27 + 1; i++){
    for(int k = 0; k < 27 + 1; k++){
      printf("%c", hashedBoard[i][k][0] + 'a');
    }
    printf("\n");
  }

  return s;
}

