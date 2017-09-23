/*------------------------------------------------------------------------------------
 Description: K-d tree and range search
 Acknowledgements: 
    http://web.stanford.edu/class/cs106l/handouts/assignment-3-kdtree.pdf
    http://www.cse.unr.edu/~bebis/CS302/Handouts/kdtree.pdf
 ------------------------------------------------------------------------------------*/
#include "kd-tree.h"

POINT* get_point_array (int size) {
    POINT *p;
    if ((p=(POINT*) calloc (sizeof(POINT), size)) == NULL) {
        fprintf (stderr, "Ran out of memory allocating data. Aborting!\n");
        exit(1);
    }
    return p;
}

POINT get_point (double x, double y) {
    POINT p;
    p.x = x;
    p.y = y;
    return p;
}

// compare functions required for qsort
int compare_y (const void* a, const void* b) {
    double diff;
    diff =  (((POINT*)a)->y - ((POINT*)b)->y);
    if (diff==0.0)
        return 0;
    else
        return (diff<0)? -1: 1;
}

int compare_x (const void* a, const void* b) {
    double diff;
    diff =  (((POINT*)a)->x - ((POINT*)b)->x);
    if (diff==0.0)
        return 0;
    else
        return (diff<0)? -1: 1;
}

NODEPTR get_node (POINT p) {
    NODEPTR n;
    if ((n=(NODEPTR) malloc(sizeof(struct node)))==NULL) {
        fprintf (stderr, "Ran out of memory allocating data. Aborting!\n");
        exit(1);
    }
    n->p = p;
    n->left = n->right = NULL;
    return n;
}

// builds a 2-d tree from a array of points
NODEPTR build2dTree (POINT p[], int start, int end, LEVEL l) {
    NODEPTR n;
    int median_index;
    if (start==end) {                   // base case when there is only one point
        n = get_node(p[start]);
        return n;
    }
    if (l==horizontal)                  // sort by x co-ordinate to find median
        qsort (p+start, end+1-start, sizeof(POINT), compare_x);
    else                                // sort by y co-ordinate to find median
        qsort (p+start, end+1-start, sizeof(POINT), compare_y);

    median_index = start + (end-start)/2;
    n = get_node(p[median_index]);      // assign median to the root node
    if (median_index>start)             // assign values to left subtree and switch level
        n->left = build2dTree (p, start, median_index-1, (l+1)%2);
    if (median_index<end)
        n->right = build2dTree (p, median_index+1, end, (l+1)%2);

    return n;
}

// prints a point as a tuple
void print_point (POINT p) {
    printf("%.3lf,%.3lf\n", p.x, p.y);
}

void print_tree(NODEPTR root, int level) {
	int i;
	if (root==NULL)
		return;
	print_tree(root->left, level+1);
	for (i=0; i<level; i++)
		printf("\t\t");
	print_point(root->p);
	print_tree(root->right, level+1);
}

// checks where x is in the range a-b
int is_present (double x, double a, double b) {
    return (x>=a && x<=b);
}

// prints all points of a tree
void print_all_nodes (NODEPTR root) {
    if (root->left!=NULL)
        print_all_nodes(root->left);
    print_point(root->p);
    if (root->right!=NULL)
        print_all_nodes(root->right);
}

/* Searches for points in tree within a given rectangle. First argument is root of the tree.
 * Next 4 arguments are co-oridinates of search rectangle. Next 4 arguments are interval co-ordinates 
 * of the given node of the tree (all points belonging to the tree lie in this interval). Last argument 
 * specifies how the root node divides the points in the left and right subtree (horizontal or vertical).
 */
void search2dTree (NODEPTR root, double x1, double x2, double y1, double y2, double int_x1, double int_x2, double int_y1, double int_y2, LEVEL l) {
    if (is_present((root->p).x, x1, x2) && is_present((root->p).y, y1, y2))         // check if root node is in the range
        print_point(root->p);
    
    if (root->left == NULL && root->right == NULL) {                                // return if this is a leaf node
        return;
    }

    if (root->left != NULL) {
        if (l == horizontal) {
            // if interval of left subtree is fully contained in the given range, report the subtree
            if (is_present(int_x1, x1, x2) && is_present((root->p).x, x1, x2) && is_present(int_y1, y1, y2) && is_present(int_y2, y1, y2))
                print_all_nodes (root->left);
            // if interval of left subtree intersects the given range
            else if (!(!is_present(int_x1, x1, x2) && !is_present((root->p).x, x1, x2) && !is_present(int_y1, y1, y2) && !is_present(int_y2, y1, y2)))
                search2dTree(root->left, x1, x2, y1, y2, int_x1, (root->p).x, int_y1, int_y2, (l+1)%2);
        }
        else {
            // if interval of left subtree is fully contained in the given range, report the subtree
            if (is_present(int_x1, x1, x2) && is_present(int_x2, x1, x2) && is_present(int_y1, y1, y2) && is_present((root->p).y, y1, y2))
                print_all_nodes (root->left);
            // if interval of left subtree intersects the given range
            else if (!(!is_present(int_x1,x1,x2) && !is_present(int_x2,x1,x2) && !is_present(int_y1,y1,y2) && !is_present((root->p).y,y1,y2)))
                search2dTree(root->left, x1, x2, y1, y2, int_x1, int_x2, int_y1, (root->p).y, (l+1)%2);
        }
    }
        
    if (root->right != NULL) {
        if (l == horizontal) {
            // if interval of right subtree is fully contained in the given range, report the subtree
            if (is_present((root->p).x, x1, x2) && is_present(int_x2, x1, x2) && is_present(int_y1, y1, y2) && is_present(int_y2, y1, y2))
                print_all_nodes (root->right);
            // if interval of right subtree intersects the given range
            else if (!(!is_present((root->p).x, x1, x2) && !is_present(int_x2, x1, x2) && !is_present(int_y1, y1, y2) && !is_present(int_y2, y1, y2)))
                search2dTree(root->right, x1, x2, y1, y2, (root->p).x, int_x2, int_y1, int_y2, (l+1)%2);
        }
        else {
            // if interval of right subtree is fully contained in the given range, report the subtree
            if (is_present(int_x1, x1, x2) && is_present(int_x2, x1, x2) && is_present((root->p).y, y1, y2) && is_present(int_y2, y1, y2))
                print_all_nodes (root->right);
            // if interval of right subtree intersects the given range
            else if (!(!is_present(int_x1, x1, x2) && !is_present(int_x2, x1, x2) && !is_present((root->p).y, y1, y2) && !is_present(int_y2, y1, y2)))
                search2dTree(root->right, x1, x2, y1, y2, int_x1, int_x2, int_y1, (root->p).y, (l+1)%2);
        }
    }
}
