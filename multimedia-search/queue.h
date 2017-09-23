
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct node {
	char path[PATH_MAX];         // to store pathname string of directory
	struct node *next;
} NODE;

// QUEUE functions
typedef struct {
    NODE *front;
    NODE *rear;
    int count;
} QUEUE;

void init_queue(QUEUE *q);
int is_queue_empty(QUEUE q);
int get_queue_count(QUEUE q);
int queue_add (QUEUE *q, char *path);
void queue_delete (QUEUE *q);
char* queue_peek (QUEUE q);
#endif
