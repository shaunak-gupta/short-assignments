/*------------------------------------------------------------------------------------
 Description: K-d tree and range search
Acknowledgements: 
    http://web.stanford.edu/class/cs106l/handouts/assignment-3-kdtree.pdf
    http://www.cse.unr.edu/~bebis/CS302/Handouts/kdtree.pdf
 ------------------------------------------------------------------------------------*/

#include "kd-tree.h"

int main (void) {
    int n, i;
    double x, y, x1, x2, y1, y2;
    double min_x, max_x, min_y, max_y;
    POINT *p;
    NODEPTR root;
    printf ("Enter the number of points: ");
    scanf ("%d", &n);
    p = get_point_array (n);
    for (i=0; i<n; i++) {
        printf ("Enter the x co-ordinate of %d-th point: ", i+1);
        scanf ("%lf", &x);
        printf ("Enter the y co-ordinate of %d-th point: ", i+1);
        scanf ("%lf", &y);
        p[i] = get_point(x, y);
    }
    root = build2dTree (p, 0, n-1, horizontal);     // Build 2-d tree from given point set
    qsort (p, n, sizeof(POINT), compare_x);
    min_x = p[0].x;
    max_x = p[n-1].x;

    qsort (p, n, sizeof(POINT), compare_y);
    min_y = p[0].y;
    max_y = p[n-1].y;
    free (p);               // free the point array now that the tree is complete

    printf ("\nBuilt the following tree:\n\n");
    print_tree (root, 0);
    printf ("\n\nEnter the range of x co-ordinates of the range (x1 - x2): ");
    scanf ("%lf", &x1);
    scanf ("%lf", &x2);
    printf ("\n\nEnter the range of y co-ordinates of the range (y1 - y2): ");
    scanf ("%lf", &y1);
    scanf ("%lf", &y2);
    printf ("\nFound following points in the given range: \n");
    search2dTree (root, x1, x2, y1, y2, min_x, max_x, min_y, max_y, horizontal);            // Search for the query range in the tree
    return 0;
}
