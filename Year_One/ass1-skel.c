/* Program to perform multi-digit integer arithmetic.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2023, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2023

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Blake Gillian 1455185
   Dated:     2023-08-31

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>

/* All necessary #defines provided as part of the initial skeleton */

#define INTSIZE	500	/* max number of digits per integer value */
#define LINELEN	999	/* maximum length of any input line */
#define NVARS	26	/* number of different variables */

#define CH_A     'a'    /* character 'a', first variable name */

#define ERROR	(-1)	/* error return value from some functions */
#define PROMPT	"> "	/* the prompt string for interactive input */

#define PRINT	'?'	/* the print operator */
#define ASSIGN	'='	/* the assignment operator */
#define PLUS	'+'	/* the addition operator */
#define MULT	'*'	/* the multiplication operator */
#define POWR	'^'	/* the power-of operator */
#define DIVS	'/'	/* the division operator */
#define ALLOPS  "?=+*^/"

#define CH_ZERO  '0'    /* character zero */
#define CH_ONE   '1'    /* character one */
#define CH_NINE  '9'    /* character nine */

#define CH_COM   ','    /* character ',' */
#define PUT_COMMAS 3    /* interval between commas in output values */

#define INT_ZERO 0	/* integer 0 */
#define INT_TEN  10	/* integer 10 */

/* Placeholder typedef for skeleton code
*/
/*typedef int struct longint_t;*/

struct longint_t{
	int val[INTSIZE];
	int len;
};

#define LONG_ZERO 0
#define LONG_ONE  1

#define DEBUG 0


/****************************************************************/

/* A "magic" additional function needing explicit declaration */
int fileno(FILE *);

/* Skeleton program function prototypes */

void print_prompt(void);
void print_tadaa();
void print_error(char *message);
int  read_line(char *line, int maxlen);
void process_line(struct longint_t vars[], char *line);
int  get_second_value(struct longint_t vars[], char *rhsarg,
	struct longint_t *second_value);
int  to_varnum(char ident);
void do_print(int varnum, struct longint_t *var);
void do_assign(struct longint_t *var1, struct longint_t *var2);
void do_plus(struct longint_t *var1, struct longint_t *var2);
void do_mult(struct longint_t *var1, struct longint_t *var2);
void do_powr(struct longint_t *var1, struct longint_t *var2);
void do_divsa(struct longint_t *N, struct longint_t *D, 
		struct longint_t *Q);
struct longint_t do_subt(struct longint_t *var1, 
		struct longint_t *var2, int dims[2][2]);
void do_divs(struct longint_t *var1, struct longint_t *var2);
struct longint_t restr_longint(struct longint_t *var,
	int range[2]);
void assign_len(struct longint_t *a);
void zero_vars(struct longint_t vars[]);
int less_than_eq(struct longint_t *var1,
	struct longint_t*var2, int dims[2][2]);
struct longint_t parse_num(char *rhs);

struct longint_t zero_long_t;

/****************************************************************/

/* Main program controls all the action
*/
int
main(int argc, char *argv[]) {
	/* Initialise the all zero longint_t which is global
	    This is used to initialise other longint_t's
	*/
	for (int i = 0; i < INTSIZE; i++){
		zero_long_t.val[i] = 0;
	}
	zero_long_t.len = 0;

	char line[LINELEN+1] = {0};
	struct longint_t vars[NVARS];

	zero_vars(vars);
	print_prompt();
	while (read_line(line, LINELEN)) {
		if (strlen(line) > 0) {
			/* non empty line, so process it */
			process_line(vars, line);
		}
		print_prompt();
	}

	print_tadaa();
	return 0;
}

/****************************************************************/

/* Prints the prompt indicating ready for input, but only if it
   can be confirmed that the input is coming from a terminal.
   Plus, output might be going to a file, that's why the prompt,
   if required, is written to stderr and not stdout
*/
void
print_prompt(void) {
	if (isatty(fileno(stdin))) {
		fprintf(stderr, "> ");
		fflush(stderr);
	}
}

void
print_tadaa() {
	/* all done, so pack up bat and ball and head home,
	   getting the exact final lines right is a bit tedious,
	   because input might be coming from a file and output
	   might be going to a file */
	if (isatty(fileno(stdin)) && isatty(fileno(stdout))) {
		printf("\n");
	}
	printf("ta daa!!!\n");
	if (isatty(fileno(stdin)) && !isatty(fileno(stdout))) {
		fprintf(stderr, "\n");
	}
}

void
print_error(char *message) {
	/* need to write an error message to the right place(s)
	*/
	if (isatty(fileno(stdin)) || isatty(fileno(stdout))) {
		fprintf(stderr, "%s\n", message);
		fflush(stderr);
	}
	if (!isatty(fileno(stdout))) {
		printf("%s\n", message);
	}
}

/****************************************************************/

/* Reads a line of input into the array passed as argument,
   returns false if there is no input available.
   All whitespace characters are removed on the way through.
*/
int
read_line(char *line, int maxlen) {
	int i=0, c;
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (i<maxlen && !isspace(c)) {
			line[i++] = c;
		}
	}
	line[i] = '\0';
	/* then, if the input is coming from a file or the output
	   is going to a file, it is helpful to echo the input line
	   and record what the command was */
	if (!isatty(fileno(stdin)) || !isatty(fileno(stdout))) {
		printf("%s%s\n", PROMPT, line);
	}
	return ((i>0) || (c!=EOF));
}

/****************************************************************/

/* Process a command by parsing the input line into parts
*/
void
process_line(struct longint_t vars[], char *line) {
	int varnum, optype, status;
	struct longint_t second_value;

	/* determine the LHS variable, it
	   must be first character in compacted line
	*/
	varnum = to_varnum(line[0]);
	if (varnum==ERROR) {
		print_error("invalid LHS variable");
		return;
	}

	/* more testing for validity 
	*/
	if (strlen(line)<2) {
		print_error("no operator supplied");
		return;
	}

	/* determine the operation to be performed, it
	   must be second character of compacted line
	*/
	optype = line[1];
	if (strchr(ALLOPS, optype) == NULL) {
		print_error("unknown operator\n");
		return;
	}

	/* determine the RHS argument (if one is required),
	   it must start in the third character of compacted line
	*/
	if (optype != PRINT) {
		if (strlen(line)<3) {
			print_error("no RHS supplied");
			return;
		}
		status = get_second_value(vars, line+2,
			&second_value);
		if (status==ERROR) {
			print_error("RHS argument is invalid");
			return;
		}
	}

	/* finally, do the actual operation
	*/
	if (optype == PRINT) {
		do_print(varnum, vars+varnum);
	} else if (optype == ASSIGN) {
		do_assign(vars+varnum, &second_value);
	} else if (optype == PLUS) {
		do_plus(vars+varnum, &second_value);
	// you will need to add further operators here
	} else if (optype == MULT) {
		do_mult(vars+varnum, &second_value);
	// you will need to add further operators here
	} else if (optype == POWR) {
		do_powr(vars+varnum, &second_value);
	// you will need to add further operators here
	} else if (optype == DIVS) {
		do_divs(vars+varnum, &second_value);
	// you will need to add further operators here
	} else {
		print_error("operation not available yet");
		return;
	}
	return;
}

/****************************************************************/

/* Convert a character variable identifier to a variable number
*/
int
to_varnum(char ident) {
	int varnum;
	varnum = ident - CH_A;
	if (0<=varnum && varnum<NVARS) {
		return varnum;
	} else {
		return ERROR;
	}
}

/****************************************************************/

/* Process the input line to extract the RHS argument, which
   should start at the pointer that is passed
*/
int
get_second_value(struct longint_t vars[], char *rhsarg,
			struct longint_t *second_value) {
	char *p;
	int varnum2;
	if (isdigit(*rhsarg)) {
		/* first character is a digit, so RHS is a number
		   now check the range of RHS for validity */
		for (p=rhsarg+1; *p; p++) {
			if (!isdigit(*p)) {
				/* nope, found an illegal character */
				return ERROR;
			}
		}
		/* nothing wrong, ok to convert */
		*second_value = parse_num(rhsarg);
		return !ERROR;
	} else {
		/* argument is not a number, so should be a variable */
		varnum2 = to_varnum(*rhsarg);
		if (varnum2==ERROR || strlen(rhsarg)!=1) {
			/* nope, not a variable either */
			return ERROR;
		}
		/* and finally, get that variable's value */
		do_assign(second_value, vars+varnum2);
		return !ERROR;
	}

	return ERROR;
}

/* Set the vars array to all zero values */
void
zero_vars(struct longint_t vars[]) {

	/* Create struct longint_t of all zero values. */
	for (int i=0; i<NVARS; i++) {
		do_assign(vars+i, &zero_long_t);
	}
	return;
}

/*****************************************************************
******************************************************************

Your answer to the assignment should start here, using your new
typedef defined at the top of the program. The next few functions
will require modifications because of the change of structure
used for a long_int, and then you'll need to start adding whole
new functions after you get these first ones working properly.
Try and make the new functions fit the style and naming pattern
of the existing ones, ok?

******************************************************************
*****************************************************************/

/* Create an internal-format number out of a string
NOTE: THE RETURNED VAL IS IN REVERSE ORDER i.e. 10 is 01
*/
struct longint_t
parse_num(char *rhs) {
	struct longint_t rhsv = zero_long_t;

	int i = 0;
	rhsv.len = strlen(rhs);

	/* While there is still an rhs to evaluate (not "") */
	while((i = strlen(rhs))) {
		/* Add 1 to the index rhs (get rid of the first char)
		   Assign digits backwards from i-1 for reverse order
		*/
		rhsv.val[i-1] = (int)rhs++[0]-48; //askii starts at 48
	}

	return rhsv;
}
/****************************************************************/

/* Print out a longint value
*/
void
do_print(int varnum, struct longint_t *var) {
	printf("register %c: ", varnum+CH_A);
	/* Every i=1 to var->len, since len = greatest array index -1.*/
	for (int i = 1; i <= var->len; i++){
		printf("%d", var->val[var->len-i]);
		/* Comma when multiple of 3 from numb end */
		if ((var->len-i)%3 == 0 && i!=var->len){
			printf(",");
		}
	}
	printf("\n");
}

/****************************************************************/

/* Assign a longint value, could do this with just an assignment
   statement, because structs can be assigned, but this is more
   elegant, and only copies over the array elements (digits) that
   are currently in use: var1 = var2
*/
void
do_assign(struct longint_t *var1, struct longint_t *var2) {
	var1->len = var2->len;
	/* INTSIZE (not var2->size) so that non-zero entries 
	   in var1 are erased
	*/
	for (int i = 0; i < INTSIZE; i++){
		var1->val[i] = var2->val[i];
	}
	return;
}

/****************************************************************/

/* Update the indicated variable var1 by doing an addition
   using var2 to compute var1 = var1 + var2
*/
void
do_plus(struct longint_t *var1, struct longint_t *var2) {
	int i = 0, rem = 0, pla;
	if (var1->len > var2->len) pla = var1->len;
	else pla = var2-> len;
 	while(i <= pla){
		//printf("\n%d, %d", var1->val[i], var2->val[i]);
		var1->val[i] += rem + var2->val[i];
		rem = (var1->val[i]/10);
		var1->val[i] -= rem*10; /* Usually 0*/
		i++;
		if (var1->val[i-1] != 0){
			var1->len = i;
		}
	}
	/* if (rem) ERROR!*/
}

/*****************************************************************
******************************************************************

Put your new functions below this line. Make sure you add suitable
prototypes at the top of the program.

******************************************************************
*****************************************************************/
/* Multiply two values and assign that to var1 */
void
do_mult(struct longint_t *var1, struct longint_t *var2) {
	int i = 0, j, rem = 0;
	struct longint_t sum = zero_long_t;
	while(i < var1->len){
		j = 0;
		while(j < INTSIZE-i){
			sum.val[j+i] += var1->val[i] * var2->val[j] + rem;
			rem = (sum.val[j+i]/10);
			sum.val[j+i] -= rem*10; /* Usually 0*/
			j++;
			//do_print('x', &sum);
		}
		i++;
	}
	assign_len(&sum);
	do_assign(var1, &sum);
	/* if (rem) ERROR!*/
}

/* var1 to the power of var2, assigned to var1 */
void
do_powr(struct longint_t *var1, struct longint_t *var2) {
	struct longint_t init = *var1, 
		i = zero_long_t, one = zero_long_t;
	one.len = 1;
	one.val[0] = 1;
	i = one;

	int a[2][2] = {{0, i.len}, 
		{0, var2->len}};

	/* While the indexing longint is less than the power (var2)*/
	while (less_than_eq(&i, var2, a) == 1)
	{
		do_mult(var1, &init);
		do_plus(&i, &one);
		a[0][1] = i.len;
	}
}

/* Returns the longint_t that exits between 
   two places in another longint_t
*/
struct longint_t
restr_longint(struct longint_t *var, int range[2]){
	struct longint_t a = zero_long_t;

	a.len = range[1]-range[0];
	for (int i = 0; i < a.len; i++){
		a.val[i] = var->val[i + range[0]];
	}

	return a;
}

/* Returns 1 if var1 < var2, 2 if var1 = var2, else 0 */
int
less_than_eq(struct longint_t *var1, 
		struct longint_t *var2, int dims[2][2]){
	struct longint_t a, b;
	a = restr_longint(var1, dims[0]);
	b = restr_longint(var2, dims[1]);

	/* A numb with less digits is always smaller, vice versa*/
	if (a.len < b.len) return 1;
	else if (a.len > b.len) return 0;

	int l;
	/* numbs are the same length */
	l = b.len;

	for (int i = 1; i <= a.len; i++){
		if (a.val[l-i] < b.val[l-i]) return 1; /* Less than */
		else if (a.val[l-i] > b.val[l-i]) return 0;
	}

	return 2; /* Equal */
}

/* Use dims to find a subsection of var1 and var2,
   then find sub.var1 - sub.var2
   Note: Has not been debugged for var1 < var2
*/
struct longint_t
do_subt(struct longint_t *var1, 
		struct longint_t *var2, int dims[2][2]) {
	struct longint_t a, b;

	a = restr_longint(var1, dims[1]);
	b = restr_longint(var2, dims[0]);

	if (DEBUG){
	printf("\nSUBTRACTION:\n");
	do_print('x', &a);
	do_print('x', &b);
	}

	/* l = length of largest longint_t */
	int i = 0, rem = 0, l;
	if (a.len > b.len) l = a.len;
	else l = b.len;
	a.len = 0;

	/* Could use for loop here */
 	while(i <= l){
		a.val[i] -= b.val[i] + rem;
		rem = 0;
		/* Add to remainder and re-evaluate a.val */
		if (a.val[i] < 0){
			a.val[i] += 10;
			rem++;
		}
		i++;
		if (a.val[i-1] != 0) a.len = i;
	}
	return a;
}

/* Recursive loop to calculate the quotient of N/D
   by long division
*/
void
do_divsa(struct longint_t *N, struct longint_t *D, 
		struct longint_t *Q) {
	struct longint_t tmp = zero_long_t, ptmp;
	int b[2][2] = {{0, N->len}, 
		{0, D->len}};

	/* N < D, exit recursion with N=Q=zero_long_t*/
	if (less_than_eq(N, D, b) == 1){
		return;
	}

	/* For long div. the subsection of N must be 
	   at least D.len
	*/
	int a[2][2] = {{0, D->len},
		{N->len-D->len, N->len}};

	/* Increase number of digits evaluated in N until
	   sub.N >= D
	*/
	while(less_than_eq(D, N, a) == 0 &&
			a[1][0] != 0){
		a[1][0]--;
	}

	/* Find number of times D fits into the subsection
	   of N evaluated
	*/
	int i = 0;
	while(less_than_eq(&tmp, N, a)){
		i++;
		ptmp = tmp;
		do_plus(&tmp, D);
		/* make sure that all of tmp is evaluated by
		   less_than_eq
	    */
		a[0][1] = tmp.len;
	}

	a[0][1] = ptmp.len;
	ptmp = do_subt(N, &ptmp, a);
	tmp = restr_longint(N, a[1]);

	/* Replace the digits of N with the remainder of
	   the long div. calc
	*/
	for (int i = 0; i < tmp.len; i++){
		N->val[N->len-1-i] = ptmp.val[tmp.len-1-i];
	}
	N->len -= tmp.len - ptmp.len;

	/* The quotient is the number of times D goes into sub.N */
	Q->val[a[1][0]] = i-1;
	/* Re-eval Q len */
	if (Q->len <= a[1][0]) Q->len = a[1][0]+1;

	do_divsa(N, D, Q);
}

/* Initialisation and return of recursive divsa function
   for long division
*/
void
do_divs(struct longint_t *N, struct longint_t *D) {
	struct longint_t Q = zero_long_t;

	/* Initialise the recursive loop
	   Note: Another function is needed so Q (quotient)
	     can be used
	*/
	do_divsa(N, D, &Q);

	/* Since in a/b, a is assigned the answer, a = Q */
	do_assign(N, &Q);
	return;
}

void
assign_len(struct longint_t *a){
	a->len = 0;
	for (int i = 0; i < INTSIZE; i++){
		if (a->val[i] != 0) a->len = i+1;
	}
}
// 	b=123456789
// 	b*987654321

// algorithms are fun!