/*---------------------------------------------------------------------------------
 * Description - Find the closest pair of points in a 2-D plane (deterministic algorithm with O(nlogn) complexity
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

// function to partition the array for quick sort
// p = pointer to array of points
// size = length of array
// compare = function to compare points
// order = ascending (1) or descending(-1)
int partition (POINT *p, int size, int (*compare)(POINT, POINT), int order) {
    int i, j;
    POINT temp, pivot;
    pivot = p[0];
    i = -1;
    j = size;
    while(1) {
        do {
            j--;
        } while (compare(p[j], pivot) * order > 0);
        do {
            i++;
        } while (compare(p[i], pivot) * order < 0);
        if (i<j) {
            temp = p[i];
            p[i] = p[j];
            p[j] = temp;
        }
        else
            return j;
    }
    return -1;
}

// function to sort the points using quick sort algorithm
void quick_sort (POINT *p, int size, int (*compare)(POINT, POINT), int order) {
    int pivot = partition (p, size, compare, order);
    if (pivot>0)
        quick_sort (p, pivot+1, compare, order);
    if (pivot<size-1)
        quick_sort (p+pivot+1, size-pivot-1, compare, order);   
}

int compare_horizontal (POINT a, POINT b) {
    return a.x-b.x;
}

int compare_vertical (POINT a, POINT b) {
    return a.y-b.y;
}

double get_distance (POINT a, POINT b) {
    return sqrt(sqr((double)a.x-b.x) + sqr((double)a.y-b.y));
}

POINT_PAIR brute_force_closest_pair(POINT *list, int size) {
    POINT_PAIR closest;
    double d1, d2, d3, distance_min;
    if (size == 2) {
        closest.distance = get_distance (list[0], list[1]);
        closest.a = list[0];
        closest.b = list[1];
        return closest;
    }
    if (size == 3) {
        d1 = get_distance (list[0], list[1]);
        d2 = get_distance (list[0], list[2]);
        d3 = get_distance (list[1], list[2]);
        distance_min = min(min(d1, d2), d3);
        closest.distance = distance_min;
        if (distance_min==d1) {
            closest.a = list[0];
            closest.b = list[1];
        }
        else {
            closest.a = list[2];
            if (distance_min==d2)
                closest.b = list[0];
            else
                closest.b = list[1];
        }
        return closest;
    }
    else {
        closest.distance = INF;
        return closest;
    }
}

POINT_PAIR unidimensional_closest_pair(POINT *sorted_y, int size) {
    double dist;
    int i;
    POINT_PAIR closest;
    closest.distance = sorted_y[1].y-sorted_y[0].y;
    closest.a = sorted_y[0];
    closest.b = sorted_y[1];
    for (i=2; i<size; i++) {
        dist = sorted_y[i].y-sorted_y[i-1].y;
        if (dist<closest.distance) {
            closest.a = sorted_y[i-1];
            closest.b = sorted_y[i];
            closest.distance = dist;
        }
    }
    return closest;
}

POINT_PAIR closest_pair (POINT *sorted_x, POINT *sorted_y, int size) {
    POINT_PAIR closest_left, closest_right, closest_total;
    double d1, distance_min;
    int mid, mid_x;
    int i, j, k;
    POINT *y_left, *y_right, *strip;
    // perform brute-force search for trivial cases (2 or 3 points)
    if (size <= 3)
        return brute_force_closest_pair (sorted_x, size);
    // find the median point and its x co-ordinate
    mid = (size-1)/2;
    mid_x = sorted_x[mid].x;
    while (sorted_x[mid].x == mid_x)
        mid++;
    if (mid == size) {
        mid--;
        while (mid>0 && sorted_x[mid].x == mid_x) {
            mid--;
        }
        if (mid<0) {
            return unidimensional_closest_pair(sorted_y, size);
        }
        else {
            mid_x = sorted_x[mid].x;
            mid++;
        }
    }

    // allocate new arrays for points in the left and right sections sorted by y co-ordinate
    y_left = (POINT*) calloc(mid, sizeof(POINT));
    y_right = (POINT*) calloc(size - mid, sizeof(POINT));
    if (y_left == NULL || y_right == NULL) {
        fprintf (stderr, "Error allocating space. Aborting!\n");
        exit(1);
    }

    // assign points to y_left or y_right
    i = j = k = 0;
    while (i<size) {
        if (j<mid && sorted_y[i].x <= mid_x) {
            y_left[j] = sorted_y[i];
            j++;
        }
        else {
            y_right[k] = sorted_y[i];
            k++;
        }
        i++;
    }

    // recursive calls to identify closest pair of points in the left and right sections
    closest_left = closest_pair (sorted_x, y_left, mid);
    closest_right = closest_pair (sorted_x + mid, y_right, size-mid);
    distance_min = min(closest_left.distance, closest_right.distance);
    if (distance_min == closest_left.distance)
        closest_total = closest_left;
    else
        closest_total = closest_right;

    // allocate a new array containing all points within distance_min horizontal distance from the median point
    if((strip = (POINT*) calloc(size, sizeof(POINT))) == NULL) {
        fprintf (stderr, "Error allocating space. Aborting!\n");
        exit(1);
    }
    for (i=0, k=0; i<size; i++) {
        d1 = (double)(sorted_y[i].x - mid_x);
        if (abs(d1) <= distance_min) {
            strip[k] = sorted_y[i];
            k++;
        }
    }

    // for each point in the strip, check distance with upto 7 points that follow it
    for (i=0; i<k-1; i++) {
        for (j=i+1; j<k && j<i+8; j++) {
            d1 = get_distance(strip[i], strip[j]);
            if (d1 < distance_min) {
                distance_min = closest_total.distance = d1;
                closest_total.a = strip[i];
                closest_total.b = strip[j];
            }
        }
    }
    free(strip);
    free(y_left);
    free(y_right);
    return closest_total;
}

int main(void) {
    POINT *points, *points_y;
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
    points_y = (POINT*) calloc(n, sizeof(POINT));
    if ((points == NULL) || (points_y == NULL)) {
        fprintf (stderr, "Error allocating memory. Aborting!\n");
        exit(1);
    }

    for (i=0; i<n; i++) 
        scanf ("%d %d", &points[i].x, &points[i].y);
    
    // copy the points array
    memcpy (points_y, points, n*sizeof(POINT));

    // sort points in ascending order by x co-ordinate
    quick_sort(points, n, compare_horizontal, 1);
    // sort points_y in ascending order by y co-ordinate
    quick_sort(points_y, n, compare_vertical, 1);

    closest = closest_pair (points, points_y, n);
    if ((closest.a.x > closest.b.x) || ((closest.a.x == closest.b.x) && (closest.a.y > closest.b.y))) {
        temp = closest.a;
        closest.a = closest.b;
        closest.b = temp;
    }
    printf ("\n\nDeterministic algorithm with %d points.\n", n);
    printf ("(%d, %d) and (%d, %d) with distance of %lf.\n", closest.a.x, closest.a.y, closest.b.x, closest.b.y, closest.distance);
    free(points);
    free(points_y);
    return 0;
}
