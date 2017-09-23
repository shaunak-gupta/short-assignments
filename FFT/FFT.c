/*---------------------------------------------------------------------------------
 * Name - Shaunak Gupta
 * ID - MTC1524
 * Details - Assignment 2, Problem 1
 * Description - Implement an algorithm of multiplying two polynomials using Fast Fourier Transform
 * Input -
 *  First line contains two space separated integres representing the degree m and n of polynomials P and Q respectively
 *  Second line contains m+1 space separated coefficients of polynomial P starting from coefficient of x^m to x^0
 *  Third line contains n+1 space separated coefficients of polynomial Q starting from coefficient of x^n to x^0
 * Output - 
 *  Output the degree k of the product polynomial in the first line
 *  Output space separated coeeficients of the product polynomial (starting from the highest degree coefficient) in the second line
 * Limits -
 * 2 <= m,n <= 1000000
 * All coefficients are integers with absolute value not greater than 1000000
 *---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define SIZE    1000000
#define INF     INFINITY
#define round(x)  (((x) >= 0) ? (int)(x+0.5) : (int)(x-0.5))
#define is_zero(a)  ((abs(a)) < 1e-5)

#define SIZE_ERROR_MSG  "Invalid degree of the polynomial. Aborting!\n"
#define MEM_ERROR_MSG  "Error allocating memory. Aborting!\n"

typedef struct {
    float real;
    float imag;
} COMPLEX;

COMPLEX *roots;
int size;

void quit (char *message) {
    perror(message);
    exit(1);
}

int closest_power_2 (int n) {
    int i;
    for (i=1; i<n; i=i<<1);
    return i;
}

COMPLEX *get_complex (int n) {
    COMPLEX *p;
    if ((p = (COMPLEX*)calloc(n, sizeof(COMPLEX))) == NULL)
        quit(MEM_ERROR_MSG);
    p = memset(p, 0, sizeof(*p));
    return p;
}

COMPLEX nth_root_unity(int i, int n) {
    float theta;
    COMPLEX c;
    theta = (2*M_PI*i)/n;
    c.real = cos(theta);
    c.imag = sin(theta);
    return c;
}

COMPLEX add(COMPLEX c1, COMPLEX c2) {
    COMPLEX c;
    c.real = c1.real + c2.real;
    c.imag = c1.imag + c2.imag;
    return c;
}

COMPLEX multiply(COMPLEX c1, COMPLEX c2) {
    COMPLEX c;
    c.real = c1.real*c2.real - c1.imag*c2.imag;
    c.imag = c1.real*c2.imag + c1.imag*c2.real;
    return c;
}

COMPLEX* FFT (COMPLEX *p, int len, int root) {
    int i, j, k, w, half;
    COMPLEX *c, *c_even, *c_odd, *odd, *even;

    c = get_complex(len);
    // base case
    if (len==1) {
        c[0] = p[0];
        return c;
    }

    // divide into odd and even coefficients
    half = len>>1;
    odd = get_complex(half);
    even = get_complex(half);

    for (i=0, j=0, k=0; i<len; i++) {
        if (i&1) {
            odd[j] = p[i];
            j++;
        }
        else {
            even[k] = p[i];
            k++;
        }
    }

    // conquer
    c_even = FFT(even, half, 2*root);
    c_odd = FFT(odd, half, 2*root);

    // combine
    w = 0;
    for (i=0; i<len; i++) {
        c[i] = add(c_even[i%half], multiply(roots[w], c_odd[i%half]));
        w = (w+root)%size;
    }
    free(c_even);
    free(c_odd);
    free(even);
    free(odd);
    return c;
}

int main(void) {
    int m, n;
    COMPLEX *p, *q, *r;
    int i;
    COMPLEX *P, *Q, *R;

    // read the polynomials
    scanf ("%d", &m);
    scanf ("%d", &n);
    if (m<0 || m>SIZE || n<0 || n>SIZE) 
        quit(SIZE_ERROR_MSG);

    size = closest_power_2 (m+n+1);
    p = get_complex(size);
    for (i=m; i>=0; i--)
        scanf ("%f", &p[i].real);  

    q = get_complex(size);
    for (i=n; i>=0; i--)
        scanf ("%f",&q[i].real);

    roots = get_complex(size);
    for (i=0; i<size; i++)
        roots[i] = nth_root_unity(i, size);

    P = FFT(p, size, 1);
    Q = FFT(q, size, 1);

    R = get_complex(size);
    for (i=0; i<size; i++)
        R[i] = multiply(P[i], Q[i]);

    // inverse FFT
    r = FFT(R, size, size-1);
    for (i=0; i<size; i++) {
        r[i].real /= size;
        r[i].imag /= size;
    }
    
    do {
        size--;
    } while (is_zero(r[size].real));

    printf ("%d\n", size);
    while (size>=0) {
        printf("%d ", round(r[size].real));
        size--;
    }
    printf("\n");

    free(p);
    free(q);
    free(r);
    free(P);
    free(Q);
    free(R);
    free(roots);
    return 0;
}
