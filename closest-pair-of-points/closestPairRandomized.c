/*---------------------------------------------------------------------------------
 * Description - Find the closest pair of points in a 2-D plane (randomized algorithm with expected O(n) complexity
 * Input -
 *  First line contains the total number of points (n)
 *  Next n lines contain two integer numbers each representing the x coordinate and y coordinate of a point respectively
 * Output - Output the closest pair of points and the distance between them
 * Limits -
 * 2 <= n <= 1000000
 * Both coordinates of every point are integers with absolute value not greater than 1000000000
 * No two points have the same co-ordinates
 *---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SIZE    1000003  
#define sqr(x)  ((x)*(x))
#define min(a,b)    (((a)<(b))?(a):(b))
#define max(a,b)    (((a)>(b))?(a):(b))

// constants for hashing
int p = SIZE;       // prime number
int a, b;           // randomly generated constants

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

// structure to store point and its cell information
typedef struct {
    POINT p;
    long long cell_number;
} POINT_CELL;

// linked list to store points in the dictionary
typedef struct node {
    POINT_CELL cell;
    struct node *next;
} NODE;

// dictionary to store points
NODE *dictionary[SIZE] = {};
    
void generate_hashing_constants() {
    do {
        //a = 16807;  
        a = rand() % p;
        b = rand() % p;
    } while (a==0);
}

// returns the cell identification number given its row and column (zero indexed)
long long get_cell (int row, int col) {
    long long r, c;
    r = row;
    c = col;
    if (r < c) 
        return (sqr(c) + r);
    else
        return (sqr(r+1) - (c+1));
}

int get_hash (long long cell_id) {
    int hash;
    cell_id = cell_id % p;
    hash = (((long long)a*cell_id + b) % p);
    return hash;
}

double get_distance (POINT a, POINT b) {
    return sqrt(sqr((double)a.x-b.x) + sqr((double)a.y-b.y));
}

void free_dictionary (POINT *points, int size, double grid_width) {
    int i, row, col, hash;
    long long cell_id;
    NODE *current, *next;
    for (i=0; i<size; i++) {
        row = points[i].y/grid_width;
        col = points[i].x/grid_width;
        cell_id = get_cell(row, col);
        hash = get_hash(cell_id);
        current = dictionary[hash];
        while (current != NULL) {
            next = current->next;
            free(current);
            current = next;
        }
        dictionary[hash] = NULL;
    }
}

void add_to_dictionary(POINT p, double grid_width) {
    int row, col, hash;
    long long cell_id;
    NODE *ptr, *new;

    row = p.y/grid_width;
    col = p.x/grid_width;
    cell_id = get_cell(row, col);
    hash = get_hash(cell_id);
    ptr = dictionary[hash];
    if ((new = (NODE*) malloc(sizeof(NODE))) == NULL) {
        fprintf (stderr, "Error allocating space. Aborting!\n");
        exit(1);
    }
    new->next = ptr;
    (new->cell).cell_number = cell_id;
    (new->cell).p = p;
    dictionary[hash] = new;
}

POINT_PAIR closest_pair (POINT *points, int size) {
    POINT_PAIR closest;
    int i;
    double distance_min, grid_width;
    int row, col, row_from, row_to, col_from, col_to;
    int j, r, c, hash, flag;
    long long cell_id;
    double distance;
    // assume the first two points to be closest
    closest.a = points[0];
    closest.b = points[1];
    distance_min = closest.distance = get_distance(points[0], points[1]);
    grid_width = distance_min/2;
    
    for (i=0; i<size; i++) {
        NODE *ptr, *new;
        row = points[i].y/grid_width;
        col = points[i].x/grid_width;
        row_from = max(0, row-2);
        col_from = max(0, col-2);
        row_to = row+2;
        col_to = col+2;
        flag = 0;
        // look for close points within current cell and 24 neighboring cells
        for (r=row_from; r<=row_to; r++) {
            for (c=col_from; c<=col_to; c++) {
                // get cell identification number and its hash value
                cell_id = get_cell(r, c);
                hash = get_hash(cell_id);
                for (ptr = dictionary[hash]; ptr!= NULL; ptr=ptr->next) {
                    if ((ptr->cell).cell_number == cell_id) {
                        distance = get_distance(points[i], (ptr->cell).p);

                        // point found closer than current minimum distance
                        if (distance < distance_min) {
                            distance_min = distance;
                            flag = 1;
                            new = ptr;
                        }
                    }
                }
            }
        }

        if (flag==1) {
            // update the new closest pair
            closest.a = points[i];
            closest.b = (new->cell).p;
            closest.distance = distance_min;

            // remove the existing i-1 points from dictionary
            free_dictionary(points, i, grid_width);
            // update the grid width
            grid_width = distance_min/2;
            // generate random values for a and b (constants for hashing)
            generate_hashing_constants();

            // re-populate the grid with points from 0 to i
            for (j=0; j<=i; j++) {
                add_to_dictionary(points[j], grid_width);
            }
        }
        else {
            add_to_dictionary(points[i], grid_width);
        }
    }
    free_dictionary(points, size, grid_width);
    return closest;
}

int main(void) {
    POINT *points;
    POINT_PAIR closest;
    POINT temp;
    int n, i, j;
    int min_x=0, min_y=0;
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

    for (i=0; i<n; i++) {
        scanf ("%d %d", &points[i].x, &points[i].y);
        if (points[i].x < min_x)
            min_x = points[i].x;
        if (points[i].y < min_y)
            min_y = points[i].y;
    }
    
    // seed the random number generator
    srand(time(NULL));

    // generate random values for a and b (constants for hashing)
    generate_hashing_constants();

    // shuffle the points by Knuth shuffle algorithm
    // also translate the points by min_x and min_y
    for (i=0; i<n-1; i++) {
        j = i + ((rand() % p) % (n-i));
        // swap points[i] and points[j]
        temp = points[i];
        points[i] = points[j];
        points[j] = temp;
        // translate the point to first quadrant
        points[i].x -= min_x;
        points[i].y -= min_y;
    }
    points[n-1].x -= min_x;
    points[n-1].y -= min_y;

    closest = closest_pair (points, n);
    // translate the points back
    closest.a.x += min_x;
    closest.a.y += min_y;
    closest.b.x += min_x;
    closest.b.y += min_y;

    if ((closest.a.x > closest.b.x) || ((closest.a.x == closest.b.x) && (closest.a.y > closest.b.y))) {
        temp = closest.a;
        closest.a = closest.b;
        closest.b = temp;
    }
    printf ("\n\nRandomized algorithm with %d points.\n", n);
    printf ("(%d, %d) and (%d, %d) with distance of %lf.\n", closest.a.x, closest.a.y, closest.b.x, closest.b.y, closest.distance);
    free(points);
    return 0;
}
