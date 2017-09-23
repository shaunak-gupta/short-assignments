/*------------------------------------------------------------------------------------
 Description: Simulating tower of hanoi using stacks
 Acknowledgements: https://en.wikipedia.org/wiki/Tower_of_Hanoi
 ------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "tower.h"

int is_empty (STACK s) {
    return (s.count == 0);
}

NODEPTR get_node (int num) {
    NODEPTR new;
    if ((new=(NODEPTR) malloc (sizeof(struct node))) == NULL) {
        fprintf (stderr, "Ran out of space. Aborting.\n");
        exit(1);
    }
    new->val = num;                 // Initialize new node
    new->next = NULL;
    return new;
}

STACK* init_stack () {
    STACK* s;
    if ((s=(STACK*) malloc (sizeof(STACK))) == NULL) {
        fprintf (stderr, "Ran out of space. Aborting.\n");
        exit(1);
    }
    s->top = NULL;       // Initialize top and count
    s->count = 0;
    return s;
}

void push (STACK *s, int num) {
    NODEPTR new;
    new = get_node (num);
    new->next = s->top;
    s->top = new;          // Update top counter and increment count
    (s->count)++;
}

int pop (STACK *s) {
    NODEPTR temp;
    int num;
    if (is_empty(*s)) {
        fprintf (stderr, "Cannot delete node. Stack is empty.\n");
        return BADVAL;
    }
    temp = s->top;
    s->top = s->top->next;
    num = temp->val;
    free (temp);
    (s->count)--;
    return num;
}

int make_move (STACK *a, STACK *b) {
    int n;
    if (!is_empty(*a) && (is_empty(*b) || a->top->val < b->top->val)) {
        n = pop(a);         // remove from a and push to b
        push (b, n);
        return 1;
    }
    if (!is_empty(*b) && (is_empty(*a) || b->top->val < a->top->val)) {
        n = pop(b);         // remove from b and push to a
        push (a, n);
        return 1;
    }
    return 0;               // no move possible
}

void print_disk (int num, int total) {          // prints a single disk on a stack (used below to print tower)
    int i;
    for (i=1; i<2*total; i++) {
        printf("%c", (i<=total-num || i>=total+num)? ' ' : '|');
    }
}

void print_towername (char name, int total) {   // prints the label of the tower
    printf ("\t%*c%*c", total, name, total, '\0');
}

/* prints the towers given their stack representations and total number of disks */
void print_towers (STACK a, STACK b, STACK c, int n) {
    NODEPTR p1, p2, p3;
    int i;
    p1 = a.top;
    p2 = b.top;
    p3 = c.top;
    printf ("\n");
    for (i=n; i>0; i--) {
        printf ("\t");
        if (i>a.count)                  // print empty spaces for no disk
            print_disk (0, n);
        else {                          // print current disk and move to next disk
            print_disk (p1->val, n);
            p1 =p1->next;
        }
        printf ("\t");                  // similarly print b and c towers
        if (i>b.count)
            print_disk (0, n);
        else {
            print_disk (p2->val, n);
            p2 =p2->next;
        }
        printf ("\t");
        if (i>c.count)
            print_disk (0, n);
        else {
            print_disk (p3->val, n);
            p3 =p3->next;
        }
        printf ("\n");
    }
    // print labels
    print_towername ('A', n);
    print_towername ('B', n);
    print_towername ('C', n);
    printf ("\n");
}
