/*---------------------------------------------------------------------------------
 * Description - Generates a random set of points in a 2-D plane
 * Input -
 *  First line contains the total number of points (n)
 *  Second line contains the maximum absolute value of co-ordinates (max)
 * Output - 
 *  First line contains the total number of points (n)
 *  Next n lines contain the x co-ordinate and y co-ordinate of each the points separated by space
 *  No two points are the same
 * Limits -
 *  2 <= n <= 1000000
 *  0 <= max <= 1000000000
 *---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SIZE    1000000
#define sqr(x)  ((x)*(x))
#define min(a,b)    (((a)<(b))?(a):(b))
#define max(a,b)    (((a)>(b))?(a):(b))

// constants for hashing
int p = 1000003; // prime numbers
int a, b;           // randomly generated constants

char boolean[SIZE] = {'N'};

// returns a single value given x and y co-ordinates
long long get_val (int x, int y) {
    long long r, c;
    r = x;
    c = y;
    if (r < c) 
        return (sqr(c) + r);
    else
        return (sqr(r+1) - (c+1));
}

int get_hash (long long val) {
    val = val%p;
    int hash = (((long long)a*val + b) % p) % SIZE;
    return hash;
}

int main(void) {
    int i, n, max;
    int x, y, sign1, sign2, hash;
    long long val;

    memset(boolean, 'N', sizeof(boolean));
    scanf ("%d", &n);
    scanf ("%d", &max);
    printf ("%d\n", n);
    
    // seed the random number generator
    srand(time(NULL));

    // generate random values for a and b (constants for hashing)
    do {
        a = rand() % p;
        b = rand() % p;
    } while (a==0);

    i = 0;
    while (i<n) {
        x = rand() % max;
        y = rand() % max;
        sign1 = rand() %2;
        sign2 = rand() %2;

        val = get_val(x, y);
        hash = get_hash(val);

        if (boolean[hash] == 'N') {
            if (sign1)
                x = -1*x;
            if (sign2)
                y = -1*y;
            printf("%d %d\n", x, y);
            boolean[hash] = 'Y';
            i++;
        }
    }
    return 0;
}
