#include "list.h"

List create_list()
{
    List list;
    list.front = NULL;
    list.back = NULL;
    list.length = 0;
    return list;
}

void list_push(List* list, ListData data)
{
    ListNode* target_node = (ListNode *) malloc(sizeof(ListNode));
    target_node->data = data;
    target_node->prev = NULL;
    target_node->next = NULL;

    if (list->length == 0)
    {
        list->front = target_node;
        list->back = target_node;
    }
    else
    {
        list->back->next = target_node;
        target_node->prev = list->back;
        list->back = target_node;
    }
    list->length += 1;
}

ListData list_pop(List* list)
{
    if (list->length == 0) return NULL;

    ListNode* target_node = list->back;
    ListData target_data = target_node->data;

    if (list->length == 1)
    {
        list->front = NULL;
        list->back = NULL;
    }
    else
    {
        ListNode* next_back_node = list->back->prev;
        list->back = next_back_node;
        next_back_node->next = NULL;
    }

    free(target_node);
    list->length -= 1;

    return target_data;
}

ListData list_dequeue(List* list)
{
if (list->length == 0) return NULL;

    ListNode* target_node = list->front;
    ListData target_data = target_node->data;

    if (list->length == 1)
    {
        list->front = NULL;
        list->back = NULL;
    }
    else
    {
        ListNode* next_front_node = list->front->next;
        list->front = next_front_node;
        next_front_node->prev = NULL;
    }

    free(target_node);
    list->length -= 1;

    return target_data;
}

ListData list_get_data(List* list, int i)
{
    if (list->length - 1 < i || i < 0) return NULL;

    ListNode* target_node;
    int index;
    if (i < list->length / 2)
    {
        target_node = list->front;
        for (index = 0; index < i; index++)
        {
            target_node = target_node->next;
        }
    }
    else
    {
        target_node = list->back;
        for (index = 0; index < list->length - i; index++)
        {
            target_node = target_node->prev;
        }
    }

    return target_node->data;
}

void list_for_each(List* list, void callback(ListData, int))
{
    int i = 0;
    ListNode* currentNode = list->front;
    while(currentNode != NULL)
    {
        callback(currentNode->data, i);
        currentNode = currentNode->next;
        i++;
    }
}

int list_get_length(List* list)
{
    return list->length;
}
