/*------------------------------------------------------------------------------------
 Description: Threaded tree traversal
 Acknowledgements: http://www.cs.berkeley.edu/~kamil/teaching/su02/080802.ppt
http://www.dcs.bbk.ac.uk/~trevor/FoC/NOTES/notes2%20trees%20p17_22.pdf
 ------------------------------------------------------------------------------------*/

#include "tree.h"

// finds index of the character c in the string s
int find_index(char c, char *s) {
	int i=0;
	while(s[i] != c)
		i++;
	return i;
}

// generates a binary tree given its preorder and inorder representations
NODEPTR create_tree(char *preorder, int start, int end, char *inorder) {
	NODEPTR root;
	int start_left, end_left;
	int start_right, end_right;
	if (start>end)
		return NULL;
	root = (NODEPTR) malloc(sizeof(struct node));
	root->val = preorder[start];            // first element in pre-order is the root
    /* all elements in the left subtree will be to the left of root element in in-order arrangement
     * similarly all elements in the right subtree will be to the right of root element
     * using this logic to get elements belonging to left and right subtrees and then
     * calling this function recursively to generate the tree
     */
	start_left = start+1;
	end_left=start;
	while(find_index(preorder[end_left+1], inorder) < find_index(root->val, inorder))
		end_left++;
	start_right = end_left+1;
	end_right = end;
	root->left = create_tree(preorder, start_left, end_left, inorder);
    root->lthread='0';          // set all thread flags to false in the beginning
	root->right = create_tree(preorder, start_right, end_right, inorder);
    root->rthread='0';
	return root;
}

// Add threads to the tree using a stack
void thread_tree (NODEPTR root, int count) {
	NODEPTR *stack;
    NODEPTR prev, next, current;
	int top = -1;
	stack = (struct node**) calloc (sizeof(struct node*), count);  // using a stack to get in-order elements to add threads
	current = root;
    prev = next = NULL;
	while (current != NULL || top != -1) {
		if (current != NULL) {
			top++;
			stack[top] = current;
			current = current->left;
		}
		else {
            if (prev!=NULL && prev->right==NULL) {
                prev->right = stack[top];
                prev->rthread='1';          // set thread flag to true
            }
            if (stack[top]->left==NULL) {
                stack[top]->left = prev;
                stack[top]->lthread='1';
            }
			prev = stack[top];
			current = stack[top]->right;
			top--;
		}
	}
	free (stack);
}

void print_inorder (NODEPTR root) {
    NODEPTR ctr;
    for (ctr = root; ctr!=NULL && ctr->left != NULL && ctr->lthread=='0'; ctr = ctr->left);     // keep taking the left subtree until a thread is encountered
    while (ctr!=NULL) {
       printf ("%c", ctr->val);         // print the value
       if (ctr->rthread == '1')         // if right link is a thread, jump to the inorder successor
           ctr = ctr->right;
       else {                           // otherwise set ctr to the leftmost element of the right subtree (in-order successor)
           for (ctr=ctr->right; ctr!=NULL && ctr->lthread=='0'; ctr = ctr->left);
       }
    }
}

void print_preorder (NODEPTR root) {
    NODEPTR ctr=root;
    while (ctr != NULL) {
        printf ("%c", ctr->val);        // print the root element first
        if (ctr->lthread=='0')          // if it has a left child, preorder successor is the left child
            ctr = ctr->left;
        else {                          // otherwise preorder successor is the first non-thread right child
            while (ctr->rthread=='1')
                ctr=ctr->right;
            ctr=ctr->right;
        }
    }
}

// not sure how to do post-order traversal using thread (using recursion here)
void print_postorder (NODEPTR root) {
    if (root->left != NULL && root->lthread == '0')     // first print left subtree (not including threaded links)
        print_postorder(root->left);
    if (root->right != NULL && root->rthread == '0')
        print_postorder(root->right);
    printf ("%c", root->val);
}
