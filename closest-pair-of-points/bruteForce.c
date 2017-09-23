/*---------------------------------------------------------------------------------
 * Description - Find the closest pair of points in a 2-D plane (brute-force algorithm with O(n^2) complexity
 * Input -
 *  First line contains the total number of points (n)
 *  Next n lines contain two integer numbers each representing the x coordinate and y coordinate of a point respectively
 * Output - Output the closest pair of points and the distance between them
 * Limits -
 * 2 <= n <= 1000000
 * Both coordinates of every point are integers with absolute value not greater than 1000000
 *---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define SIZE    1000000
#define INF     INFINITY
#define sqr(x)  ((x)*(x))
#define min(a,b)    (((a)<(b))?(a):(b))

// representation of point in a plane
typedef struct {
    int x;
    int y;
} POINT;

typedef struct {
    POINT a;
    POINT b;
    double distance;
} POINT_PAIR;

double get_distance (POINT a, POINT b) {
    return sqrt(sqr((double)a.x-b.x) + sqr((double)a.y-b.y));
}

POINT_PAIR closest_pair(POINT *list, int size) {
    POINT_PAIR closest;
    int i, j;
    double dist;
    closest.a = list[0];
    closest.b = list[1];
    closest.distance = get_distance(list[0], list[1]);

    for (i=0; i<size-1; i++) {
        for (j=i+1; j<size; j++) {
            dist = get_distance(list[i], list[j]);
            if (dist < closest.distance) {
                closest.a = list[i];
                closest.b = list[j];
                closest.distance = dist;
            }
        }
    }
    return closest;
}

int main(void) {
    POINT *points;
    POINT_PAIR closest;
    POINT temp;
    int n, i;
    // read the number of points and points' co-ordinates
    scanf ("%d", &n);
    if (n<=1 || n>SIZE) {
        fprintf (stderr, "Number of points should be between %d and %d. Aborting!\n", 2, SIZE);
        return 1;
    }
    
    // allocate memory for arrays of points
    points = (POINT*) calloc(n, sizeof(POINT));
    if (points == NULL) {
        fprintf (stderr, "Error allocating memory. Aborting!\n");
        exit(1);
    }

    for (i=0; i<n; i++) 
        scanf ("%d %d", &points[i].x, &points[i].y);
    
    closest = closest_pair (points, n);
    if ((closest.a.x > closest.b.x) || ((closest.a.x == closest.b.x) && (closest.a.y > closest.b.y))) {
        temp = closest.a;
        closest.a = closest.b;
        closest.b = temp;
    }
    printf ("\n\nBrute-force algorithm with %d points.\n", n);
    printf ("(%d, %d) and (%d, %d) with distance of %lf.\n", closest.a.x, closest.a.y, closest.b.x, closest.b.y, closest.distance);
    free(points);
    return 0;
}
