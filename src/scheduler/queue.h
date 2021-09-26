#ifndef _QUEUE_HEADER_FILE
#define _QUEUE_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>

typedef struct QueueNode
{
    struct QueueNode* next;
    struct QueueNode* prev;
    int data;
} QueueNode;

typedef struct Queue
{
    struct QueueNode* front;
    struct QueueNode* back;
    int length;
} Queue;

Queue create_queue();
void enqueue(Queue* queue, int data);
int dequeue(Queue* queue);
int get_length(Queue* queue);
void print_queue();

#endif
