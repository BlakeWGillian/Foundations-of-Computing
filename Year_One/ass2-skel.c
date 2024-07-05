/* Program to generate text based on the context provided by input prompts.

  Skeleton program written by Artem Polyvyanyy, http://polyvyanyy.com/,
  September 2023, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.
  All included code is (c) Copyright University of Melbourne, 2023.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: Blake Wilson Gillian 1455185
  Dated:     2023-09-24
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

/* #DEFINE'S -----------------------------------------------------------------*/
#define SDELIM "==STAGE %d============================\n"   // stage delimiter
#define MDELIM "-------------------------------------\n"    // delimiter of -'s
#define THEEND "==THE END============================\n"    // end message
#define NOSFMT "Number of statements: %d\n"                 // no. of statements
#define NOCFMT "Number of characters: %d\n"                 // no. of chars
#define NPSFMT "Number of states: %d\n"                     // no. of states
#define TFQFMT "Total frequency: %d\n"                      // total frequency

#define MAX_LINE 37                                         // max line length
#define FALSE 0

#define CRTRNC '\r'                             // carriage return character
#define DEBUG 0

/* TYPE DEFINITIONS ----------------------------------------------------------*/
typedef struct state state_t;   // a state in an automaton
typedef struct node  node_t;    // a node in a linked list
typedef struct lstring string_t;// a string in a linked list

struct node {                   // a node in a linked list of transitions has
    string_t*       str;        // ... *a transition string
    state_t*        state;      // ... *the state reached via the string, and
    node_t*         child;      // ... *a link to the first child of the node.
    node_t*         sibl;       // ... *a sibling node (for ll of children)
};

struct state {                  // a state in an automaton is characterized by
    unsigned int    id;         // ... an identifier,
    unsigned int    freq;       // ... frequency of traversal, and
    int             visited;    // ... visited status flag,
};

struct lstring {                // a string in a linked list needs
    char            c;          // ... a character, and
    string_t*       next;       // ... *the next list item
};

/* USEFUL FUNCTIONS ----------------------------------------------------------*/
int mygetchar(void);              // getchar() that skips carriage returns
void free_nod(node_t* n);        // Free malloc for inidividual node
void free_aut(node_t* head);// Free malloc for automaton starting w/ head
node_t* init_node(string_t *str, unsigned int id);
void print_nod(node_t* node);
node_t* max_unvisited_sibl(node_t* head, node_t* max_nod);
void process_str(node_t* head, string_t* s);
void free_str(string_t* head);
string_t* init_str(char c, string_t* next);
int print_str(string_t* s);
int print_str_len(string_t* s, int len);
string_t* copy_str(string_t* s);
void process_prompt(node_t* head);
int outgoing_arcs(node_t* head);
int compress(node_t* head, int steps, unsigned int *states,
    unsigned int *sum_freq);
void print_aut(node_t* head);
void push_str(string_t *a, string_t **b);
node_t* min_unvisited_sibl(node_t* head, node_t* min_nod);
int special(char a); // 1 if a is not '\n', '\0', or '\r'
void zero_visited(node_t* head);
int str_diff(string_t *a, string_t **b);
node_t* in_sibl(node_t* head, string_t **str);

/* WHERE IT ALL HAPPENS ------------------------------------------------------*/
int main(int argc, char* argv[]) {
    // Message from Artem: The proposed in this skeleton file #define's,
    // typedef's, and struct's are the subsets of those from my sample solution
    // to this assignment. You can decide to use them in your program, or if
    // you find them confusing, you can remove them and implement your solution
    // from scratch. I will share my sample solution with you at the end of
    // the subject.

    // ==STAGE 0================================================================
    unsigned int id = 0, sum_chars = 0, s = 0, stmnt_chars = 0, sum_freq = 0;

    node_t* head = init_node(NULL, 0);
    node_t* parent = head, *tmp_nod = NULL;

    string_t *stmnt = init_str('~', init_str('\0', NULL));
    string_t *stmnt_head = stmnt;

    while ((stmnt->c = mygetchar()) != -1){
        sum_chars++;
        stmnt_chars++;

        if (stmnt->c == '\n'){ // New statement
            sum_chars--; // New Line is not counted as a char
            if (stmnt_chars == 1) // End of Stage 0
                break;
            s++;
            stmnt_chars = 0; // New line - so that stages can be separated.
            parent = head;
        } else if ((tmp_nod = in_sibl(parent->child, &stmnt))){
            parent->state->freq += 1;
            sum_freq += 1;
            parent = tmp_nod;
        } else { // a[0] is a unique char that needs to be added
            parent->state->freq += 1;
            sum_freq += 1;
            id++;
            tmp_nod = parent->child;
            parent = parent->child = init_node(stmnt_head, id);
            parent->sibl = tmp_nod;
        }
        stmnt = stmnt_head;
    }
    free_str(stmnt_head);
    id++; // To include head node (ID = 0).
    printf(SDELIM, 0);
    printf(NOSFMT, s);
    printf(NOCFMT, sum_chars);
    printf(NPSFMT, id);
    if (DEBUG) print_aut(head);

    // ==STAGE 1================================================================
    printf(SDELIM, 1);
    process_prompt(head);

    // ==STAGE 2================================================================
    printf(SDELIM, 2);
    scanf("%d\n", &sum_chars);

    compress(head, sum_chars, &id, &sum_freq);
    zero_visited(head);

    printf(NPSFMT, id);
    printf(TFQFMT, sum_freq);
    printf(MDELIM);

    process_prompt(head);

    free_aut(head);
    printf(THEEND);
    return EXIT_SUCCESS;        // algorithms are fun!!!
}

/* USEFUL FUNCTIONS ----------------------------------------------------------*/

// An improved version of getchar(); skips carriage return characters.
// NB: Adapted version of the mygetchar() function by Alistair Moffat
int mygetchar() {
    int c;
    while ((c=getchar())==CRTRNC);
    return c;
}

// Processes a stage's input, and prints output.
void process_prompt(node_t* head){
    string_t *a_head = init_str('~', NULL);
    string_t *a_tail = a_head;
    int backlog = 1; // Backlog of statements without a '\n' end
    a_tail->next = NULL; // So a is not pointing to garbage
    while ((a_tail->c = mygetchar()) != -1 || (backlog--)){
        if (a_tail->c == '\n' || backlog == FALSE){ // New statement
            if (a_head->c == '\n') // End of Stage 1
                break;
            a_tail->c = '\0'; // Changing the '\n' (or '~' w/ backlog)
            process_str(head, a_head);
            free_str(a_head->next);
            a_head->next = NULL; // So a_head is not pointing to garbage
            a_tail = a_head;
            continue;
        }
        a_tail->next = init_str('~', NULL);
        a_tail = a_tail->next;
    }
    free_str(a_head);
    return;
}

// Prints the response for a given string.
void process_str(node_t* head, string_t* prompt){
    node_t* parent = head; // First Sibling
    string_t* s = prompt, *end_s = init_str('\n', NULL);
    string_t* el = init_str('.', init_str('.',
        init_str('.', end_s)));
    int len = 0, slen = 0;

    slen = print_str_len(prompt, -1); // QWUICK FIX
    while (s && len < MAX_LINE){
        if ((parent = in_sibl(parent->child, &s))){ // End of input
            if (s == NULL || (special(s->c) && s->next == NULL)){
                s = parent->str;
                for (int i = 0; i < slen-len; i++)
                    s = s->next;
                if (MAX_LINE < slen) slen = MAX_LINE;
                len += print_str_len(parent->str, slen-len);
                len += print_str_len(el, MAX_LINE-len);
                len += print_str_len(s, MAX_LINE-len);
                while (parent->state->freq != 0){ // CONSIDER DO WHILE!!!!
                    parent = parent->child;
                    parent = max_unvisited_sibl(parent, NULL);
                    len += print_str_len(parent->str, MAX_LINE-len);
                }
                break;
            }
            len += print_str_len(parent->str, MAX_LINE-len);
        }
        else{ // a[0] is a unique char that needs to be added
            free_str(s->next);
            s->next = NULL;
            len += print_str_len(s, MAX_LINE-len);
            len += print_str_len(el, MAX_LINE-len);
            break;
        }
    }
    printf("\n");
    free_str(el);
}

// Compress the automata.
int compress(node_t* head, int steps,
        unsigned int *states, unsigned int *sum_freq){
    node_t* new_y, *cld;

    if (outgoing_arcs(head->child) && steps > 0){ // X and Y
        if (outgoing_arcs(head) == 1){
            cld = head->child;
            *sum_freq -= head->child->state->freq;
            *states -= 1;
            new_y = head->child = head->child->child;
            do {
                push_str(cld->str, &new_y->str);
            } while ((new_y = new_y->sibl) != NULL);
            free_nod(cld); // Y node is no longer needed
            steps--;
            steps = compress(head, steps, states, sum_freq);
        }
        else { // Head has >1 outgoing arcs.
            while ((new_y = min_unvisited_sibl(head->child, NULL))){
                steps = compress(new_y, steps, states, sum_freq);
            }
        }
    }
    head->state->visited = 1;
    return steps; // No steps or compression left.
}

// Changes the string_t b, such that b = a + b;
void push_str(string_t *fr, string_t **en){
    if (fr == NULL)
        return;
    // NB: fr is freed later in compression, with its node.
    string_t *frc = copy_str(fr);
    fr = frc;
    while (frc->next != NULL){
        if (frc->next->next == NULL || special(frc->next->c)){
            free_str(frc->next);
            frc->next = *en;
            *en = fr;
            return;
        }
        frc = frc->next;
    }
}

// Initiate a node.
node_t* init_node(string_t *str, unsigned int id){
    node_t* n = malloc(sizeof(node_t));
    n->state = malloc(sizeof(state_t));
    n->state->id = id;
    n->state->freq = 0;
    n->state->visited = FALSE;
    n->child = NULL;
    n->sibl = NULL;
    n->str = copy_str(str);
    return n;
}

// Initiate a string_t.
string_t* init_str(char c, string_t* next){
    string_t* str = malloc(sizeof(string_t));
    str->c = c;
    str->next = next;
    return str;
}

// Return a duplicate a string_t with a new address.
string_t* copy_str(string_t* str){
    if (str == NULL) // More reliable than s->c == \0
        return NULL;
    return init_str(str->c, copy_str(str->next));
}

// Free a node.
void free_nod(node_t* node){
    free_str(node->str);
    free(node->state);
    free(node);
}

// Free a string_t.
void free_str(string_t* head){
    if (head){
        free_str(head->next);
        free(head);
    }
}

// Free all nodes in an automata
void free_aut(node_t* head){
    if (head){
        free_aut(head->child);
        free_aut(head->sibl);
        free_nod(head);
    }
}

// Return the sibling that contains str
node_t* in_sibl(node_t* head, string_t **str){
    node_t* tmp = NULL;
    // tmp needed so that recursive calls are not limited
    // (i.e. head->sibl can't see head)
    tmp = max_unvisited_sibl(head, NULL);
    if (tmp == NULL)
        return NULL;
    else if (str_diff(tmp->str, str) != 0){ // Str not in tmp
        tmp->state->visited = 1;
        tmp =  in_sibl(head, str);
    }
    zero_visited(head);
    return tmp;
}

// Makes all the 'visited' tags 0 of an automata.
void zero_visited(node_t* head){
    if (head){
        head->state->visited = 0;
        zero_visited(head->sibl);
        zero_visited(head->child);
    }
}

// Returns the max_nod unvisited node among siblings.
node_t* max_unvisited_sibl(node_t* head, node_t* max_nod){ // FREQ then ASCII
    if (head == NULL) // Not NULL or head of automata
        return max_nod;
    else if (head->state->visited != FALSE)
        return max_unvisited_sibl(head->sibl, max_nod);
    else if (max_nod == NULL || head->state->freq > max_nod->state->freq)
        return max_unvisited_sibl(head->sibl, head);
    else if (str_diff(head->str, &max_nod->str) > 0)
            return max_unvisited_sibl(head->sibl, head);
    return max_unvisited_sibl(head->sibl, max_nod);
}

// Returns the min_nod unvisited node among siblings.
node_t* min_unvisited_sibl(node_t* head, node_t* min_nod){ // ASCII
    if (head == NULL) // Not NULL or head of automata
        return min_nod;
    else if (head->state->visited != FALSE)
        return min_unvisited_sibl(head->sibl, min_nod);
    else if (min_nod == NULL)
        return min_unvisited_sibl(head->sibl, head);
    else if (str_diff(head->str, &min_nod->str) < 0)
            return min_unvisited_sibl(head->sibl, head);
    return min_unvisited_sibl(head->sibl, min_nod);
}

// Last chars of strings a - b
int str_diff(string_t *a, string_t **tmp){
    string_t* b = *tmp;
    while (a && b && a->next && b->next){
        if (a->c != b->c)
            return a->c - b->c;
        b = b->next;
        a = a->next;
    }
    *tmp = b;
    return 0; // No diff
}

// Number of outgoing arcs from head
int outgoing_arcs(node_t* head){
    if (head && head->child){
        if (head->child->sibl)
            return 2; // Greater that one arc
        return 1;
    }
    return 0; // No children
}

// Returns the length of the string printed, prints string
int print_str_len(string_t* str, int maxlen){
    if(str && maxlen != 0){
        if (special(str->c) == 0){
            if (maxlen > 0) printf("%c", str->c);
            maxlen--;
            return print_str_len(str->next, maxlen)+1;
        }
    }
    return 0;
}

// Returns 0 if a is special, else 1
int special(char a){
    if (a == '\n' || a == '\0' || a == '\r')
        return 1;
    return 0;
}
/* THE END -------------------------------------------------------------------*/