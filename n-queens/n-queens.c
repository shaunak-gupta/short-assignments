/*-----------------------------------------------------------------------------
 Description - Solves (tries to) n-queens for a given n
------------------------------------------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>

#define abs_diff(a,b)	(((a)>(b))? ((a)-(b)) : ((b)-(a)))

typedef struct node {
	int val;
	struct node *next;
} STACK_NODE;

typedef struct {
	STACK_NODE *top;
	int count;
} STACK;

void init_stack(STACK *s) {
	s->top = NULL;
	s->count = 0;
}

int is_stack_empty(STACK s) {
	return (s.top==NULL);
}

int stack_push(STACK *s, int n) {	// Takes address of stack so that it may update it
	STACK_NODE *temp = (STACK_NODE*) malloc(sizeof(STACK_NODE));
	if (temp==NULL)
		return -1;
	temp->val = n;
	temp->next = s->top;
	s->top = temp;
	(s->count)++;
	return 1;
}

int stack_pop(STACK *s) {		// Takes address of stack so that it may update it
	STACK_NODE *temp;
	int n;
	if (s->top == NULL) {
		fprintf(stderr, "Stack empty. Cannot pop element.\n");
		return -1;
	}
	temp = s->top;
	s->top = s->top->next;
	n = temp->val;
	free(temp);
	(s->count)--;
	return n;
}

int get_stack_count(STACK s) {
	return s.count;
}

/* determines if any of existing queens in the stack are attacking a queen if (count+1)th row at pos column */
int is_attacking_position(STACK s, int pos) {
	STACK_NODE *ctr;
	int diff;				// Keeps track of difference in rows match diagonals
	for (ctr = s.top, diff = 1; ctr != NULL; ctr = ctr->next, diff++) {
		if (ctr->val == pos || abs_diff(ctr->val, pos) == diff)
			return 1;
	}
	return 0;
}

void print_board(STACK s, int size) {
	int i, j;
	STACK_NODE *ctr, *temp;
	ctr = s.top;
	while (ctr!=NULL) {
		printf("%c", (ctr==s.top)? ' ' : '|');
		for (j=0; j<size; j++)
			printf("___%c", (ctr==s.top)? ' ' : '|');
		printf("\n|");
		for (i=0; i<size;i++) {
			printf(" %c |", (ctr->val == i+1)? 'X' : ' '); ;
		}
		printf("\n");
		temp = ctr;
		ctr = ctr->next;
		free(temp);
	}
	printf("|");
	for (j=0; j<size; j++)
		printf("___|");
	printf("\n");
}

int main(void) {
	int n, ctr;
	STACK s;
	printf ("Enter the value of n: ");
	scanf ("%d", &n);
	init_stack(&s);			// Level of stack indicates the row number and its value indicates column number (1 to n)
	ctr = 1;
	stack_push(&s, ctr);
	while (get_stack_count(s) < n) {
		while (is_attacking_position(s, ctr) && ctr<= n)
			ctr++;
		if (ctr <=n) {
			stack_push(&s, ctr);
			ctr = 1;
		}
		else {
			if (get_stack_count(s) > 0)
				ctr = 1 + stack_pop(&s);
			else {
				fprintf(stderr, "Did not find a solution. Aborting!\n");
				return 1;
			}
		}
	}
	printf ("Found a solution.\n");
	print_board(s,n);
	return 0;	
}
