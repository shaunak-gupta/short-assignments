#include "queue.h"

void init_queue(QUEUE *q) {
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

int is_queue_empty(QUEUE q) {
    return (q.count==0);
}
int get_queue_count(QUEUE q) {
    return q.count;
}

/* adds a new element at the rear */
int queue_add (QUEUE *q, char *path) {
    NODE* el = (NODE*) malloc(sizeof(NODE));
    if (el == NULL)             // malloc unsuccessfull, return
        return -1;
    strcpy (el->path, path);
    el->next = NULL;
    if (q->rear != NULL)        // If queue is not already empty, add new element behind current rear
        q->rear->next = el;
    else
        q->front = el;
    q->rear = el;
    (q->count)++;
    return 1;
}

/* deletes the element at the front of the queue. also updates front by updating value at its address */
void queue_delete (QUEUE *q) {
    NODE* el;
    el = q->front;
    if (el == NULL) {
        fprintf(stderr, "Cannot delete element. Queue is empty!\n");
        return;
    }
    q->front = el->next;
    free(el);
    (q->count)--;
    if (q->front==NULL)
        q->rear = NULL;
}

// returns the path of the element at the front. Doesn't delete that element
char* queue_peek (QUEUE q) {
    if (q.front == NULL)
        return NULL;
    return q.front->path;
}
