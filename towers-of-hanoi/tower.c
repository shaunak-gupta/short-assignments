/*------------------------------------------------------------------------------------
 Description: Simulating tower of hanoi using stacks
 Acknowledgements: https://en.wikipedia.org/wiki/Tower_of_Hanoi
 ------------------------------------------------------------------------------------*/

#include <stdio.h>
#include "tower.h"

int main (void) {
    STACK *A, *B, *C;
    int n, i;
    printf ("Enter the total number of disks (n): ");
    scanf ("%d", &n);

    A = init_stack();
    B = init_stack();
    C = init_stack();

    // fill A with n disks (each value representing the radius of the disk */
    for (i=n; i>0; i--)
        push (A, i);

    printf ("Initial arrangement:\n");
    print_towers (*A, *B, *C, n);
    
    i = 1;

    if (n%2 == 0) {             // If n is even
        while (C->count < n) {  // repeat until stack C is not full
            make_move(A, B);
            printf ("\n%d-th move:", i); i++;
            print_towers (*A, *B, *C, n);

            make_move(A, C);
            printf ("\n%d-th move:", i); i++;
            print_towers (*A, *B, *C, n);
            if (C->count==n)
                break;

            make_move(B, C);
            printf ("\n%d-th move:", i); i++;
            print_towers (*A, *B, *C, n);
        }
    }
    else {             // If n is odd
        while (C->count < n) {  // repeat until stack C is not full
            make_move(A, C);
            printf ("\n%d-th move:", i); i++;
            print_towers (*A, *B, *C, n);
            if (C->count==n)
                break;

            make_move(A, B);
            printf ("\n%d-th move:", i); i++;
            print_towers (*A, *B, *C, n);
            
            make_move(C, B);
            printf ("\n%d-th move:", i); i++;
            print_towers (*A, *B, *C, n);
        }
    }
    return 0;
}
