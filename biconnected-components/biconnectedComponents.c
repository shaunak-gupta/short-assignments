/*---------------------------------------------------------------------------------
 * Description - Find the biconnected components in a graph (with O(m+n) complexity
 * Input -
 *  First line contains the total number of points (n)
 * Output - Output the biconnected components of the graph
 * Limits -
 * 2 <= n <= 200
 *---------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SIZE    200
#define PROB    0.1
#define min(a,b)    (((a)<(b))?(a):(b))

char adjacency_matrix[SIZE][SIZE];
char visited[SIZE];
int parent[SIZE];
int dfs_number[SIZE], low_number[SIZE];
int** stack;
int stack_count = 0;
int count_visited = 0;
int n, m;

void push_edge_stack(int u, int v) {
    stack[stack_count][0] = u; 
    stack[stack_count][1] = v;
    stack_count++;
}

void print_component(int u, int v) {
    printf ("\nNew biconnected component-\n");
    do {
        // pop edge from stack and print it
        stack_count--;
        printf("%d--%d ", stack[stack_count][0], stack[stack_count][1]);
    } while (stack[stack_count][0] != u || stack[stack_count][1] != v);
    printf("\n");
}

void dfs_visit (int vertex) {
    int i;
    visited[vertex] = '1';
    count_visited++;

    dfs_number[vertex] = low_number[vertex] = count_visited;
    
    // find the vertices adjacent to vertex
    for (i=0; i<n; i++) {
        // if an edge exists
        if (adjacency_matrix[vertex][i] == '1') {
            if (visited[i] == '0') {
                // add this edge to stack
                push_edge_stack(vertex, i);

                // perform dfs
                parent[i] = vertex;
                dfs_visit(i);

                // if vertex is an articulation point
                if (low_number[i] >= dfs_number[vertex])
                    print_component(vertex, i);
                low_number[vertex] = min(low_number[vertex], low_number[i]);
            }
            // if this edge is a back edge
            else if (parent[vertex] != i && dfs_number[i] < dfs_number[vertex]) {
                push_edge_stack(vertex, i);
                low_number[vertex] = min(low_number[vertex], dfs_number[i]);
            }
        }
    }
}

int main(void) {
    int i, j;

    // initialize visited, parent and adjacency matrix
    memset(adjacency_matrix, '0', sizeof(adjacency_matrix));
    memset(visited, '0', sizeof(visited));
    memset(parent, -1, sizeof(parent));
    memset(dfs_number, 0, sizeof(dfs_number));
    memset(low_number, 0, sizeof(low_number));

    // read the number of vertices
    scanf ("%d", &n);
    if (n<=1 || n>SIZE) {
        fprintf (stderr, "Number of vertices should be between %d and %d. Aborting!\n", 2, SIZE);
        return 1;
    }

    // seed random number generator
    srand(time(NULL));
    m = 0;

    printf ("Following edges are present-\n");
    // define the graph (add random edges with given probability)
    for (i=0; i<n-1; i++) {
        for (j=i+1; j<n; j++) {
            if (random()*1.0/RAND_MAX < PROB) {
                adjacency_matrix[i][j] = '1';
                adjacency_matrix[j][i] = '1';
                m++;
                printf("%d--%d\n", i, j);
            }
        }
    }

    // initialize the stack
    if ((stack = (int**) calloc(m, sizeof(int*))) == NULL) {
        fprintf (stderr, "Error allocating stack. Aborting!\n");
        exit(1);
    }
    for (i=0; i<m; i++) {
        if ((stack[i] = (int*)calloc(2, sizeof(int))) == NULL) {
            fprintf (stderr, "Error allocating stack. Aborting!\n");
            exit(1);
        }
    }

    for (i=0; i<n; i++) {
        if (visited[i] == '0') {
            dfs_visit(i);
        }
    }
    return 0;
}
