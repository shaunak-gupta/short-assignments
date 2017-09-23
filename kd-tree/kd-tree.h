/*------------------------------------------------------------------------------------
 Description: K-d tree and range search
 Acknowledgements: 
    http://web.stanford.edu/class/cs106l/handouts/assignment-3-kdtree.pdf
    http://www.cse.unr.edu/~bebis/CS302/Handouts/kdtree.pdf
 ------------------------------------------------------------------------------------*/

#ifndef _KD_TREE_H_
#define _KD_TREE_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    horizontal,
    vertical
} LEVEL;

typedef struct {
    double x;
    double y;
} POINT;

struct node {
    POINT p;
    struct node *left, *right;
};

typedef struct node* NODEPTR;

POINT* get_point_array (int size);
POINT get_point (double x, double y);
int compare_y (const void* a, const void* b);
int compare_x (const void* a, const void* b);
NODEPTR get_node (POINT p);
NODEPTR build2dTree (POINT p[], int start, int end, LEVEL l);
void print_point (POINT p);
void print_tree(NODEPTR root, int level);
int is_present (double x, double a, double b);
void print_all_nodes (NODEPTR root);
void search2dTree (NODEPTR root, double x1, double x2, double y1, double y2, double int_x1, double int_x2, double int_y1, double int_y2, LEVEL l);
#endif
