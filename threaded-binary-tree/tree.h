/*------------------------------------------------------------------------------------
 Description: Threaded tree traversal
 Acknowledgements: http://www.cs.berkeley.edu/~kamil/teaching/su02/080802.ppt
http://www.dcs.bbk.ac.uk/~trevor/FoC/NOTES/notes2%20trees%20p17_22.pdf
 ------------------------------------------------------------------------------------*/

#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node {
    /* tree stores character as its value
     * it is not a binary search tree, 
     * so we are going to use in-order and pre-order arrangements to generate the tree
     */
    char val;
    struct node *left, *right;
    char lthread, rthread;           // booleans to store whether pointers are links or threads
};

typedef struct node* NODEPTR;
int find_index(char c, char *s);
NODEPTR create_tree(char *preorder, int start, int end, char *inorder);
void thread_tree (NODEPTR root, int count);
void print_inorder (NODEPTR root);
void print_preorder (NODEPTR root);
void print_postorder (NODEPTR root);
#endif
