#ifndef _LIST_HEADER_FILE
#define _LIST_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>

typedef void* ListData;

typedef struct ListNode
{
    struct ListNode* next;
    struct ListNode* prev;
    ListData data;
} ListNode;

typedef struct List
{
    struct ListNode* front;
    struct ListNode* back;
    int length;
} List;

List create_list();
void list_push(List* list, ListData data);
ListData list_pop(List* list);
ListData list_dequeue(List* list);
void list_for_each(List* list, void callback(ListData, int));
ListData list_get_data(List* list, int ith);
int list_get_length(List* list);
void list_print_queue(List* list);

#endif
