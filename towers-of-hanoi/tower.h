/*------------------------------------------------------------------------------------
 Description: Simulating tower of hanoi using stacks
 Acknowledgements: https://en.wikipedia.org/wiki/Tower_of_Hanoi
 ------------------------------------------------------------------------------------*/

#ifndef _TOWER_H_
#define _TOWER_H_
#define BADVAL  -9999

struct node {
    int val;
    struct node *next;
};

typedef struct node* NODEPTR;

typedef struct {
    NODEPTR top;
    int count;
} STACK;


int is_empty (STACK s);

NODEPTR get_node (int num);

STACK* init_stack ();

void push (STACK *s, int num);

int pop (STACK *s);

int make_move (STACK *a, STACK *b);

void print_disk (int num, int total);

void print_towername (char name, int total);

void print_towers (STACK A, STACK B, STACK C, int n);

#endif
