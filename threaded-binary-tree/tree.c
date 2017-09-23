/*------------------------------------------------------------------------------------
 Description: Threaded tree traversal
 Acknowledgements: http://www.cs.berkeley.edu/~kamil/teaching/su02/080802.ppt
http://www.dcs.bbk.ac.uk/~trevor/FoC/NOTES/notes2%20trees%20p17_22.pdf
 ------------------------------------------------------------------------------------*/

#include "tree.h"

int main(void) {
	char *preorder, *inorder;       // trees contain a single character in a node (value)
	struct node *root;
	int count;
    // First take input pre-order and in-order representations to generate the tree
	printf("Enter pre-order representation of tree as a string (Nodes contain a single character): ");
	scanf("%ms", &preorder);
	printf("Enter in-order representation of tree as a string (Nodes contain a single characer): ");
	scanf("%ms", &inorder);

	count = strlen(preorder);   // total number of nodes in the tree

	root = create_tree(preorder, 0, count-1, inorder);  // generate tree

	thread_tree(root, count);
	
    printf ("\nIn-oder traversal using threads: ");
    print_inorder (root);

    printf ("\nPre-order traversal using threads: ");
    print_preorder (root);
    
    printf ("\nPost-order traversal using recursion: ");
    print_postorder (root);
    printf("\n");
	return 0;
}
