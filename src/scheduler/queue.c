#include "queue.h"

Queue create_queue()
{
    Queue queue;
    queue.front = NULL;
    queue.back = NULL;
    queue.length = 0;
    return queue;
}

void enqueue(Queue* queue, int data)
{
    QueueNode* node = (QueueNode *) malloc(sizeof(QueueNode));
    node->data = data;
    node->prev = queue->back;
    node->next = NULL;

    if (queue->length > 0) queue->back->next = node;
    else queue->front = node;
    
    queue->back = node;

    queue->length += 1;
}

int dequeue(Queue* queue)
{
    if (queue->length < 1) { printf("[ERROR] queue has no node"); return -1; }

    QueueNode* target = queue->front;

    int result = target->data;

    if (queue->length > 1)
    {
        queue->front = queue->front->next;
        queue->front->prev = NULL;
    } else
    {
        queue->front = NULL;
        queue->back = NULL;
    }
    free(target);

    queue->length -= 1;

    return result;
}

int get_length(Queue* queue)
{
    return queue->length;
}

void print_queue(Queue* queue)
{
    QueueNode* pointer = queue->front;
    printf("queue(%d) ", get_length(queue));
    while(pointer != NULL)
    {
        printf(" %d ", pointer->data);
        pointer = pointer->next;
    }
    printf("\n");
}
