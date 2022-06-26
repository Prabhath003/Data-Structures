#include "node.h"

iterators* init()
{
    iterators* temp;
    // dynamically allocating the space for iterators to temp pointer
    temp = (iterators*)malloc(sizeof(iterators));
    // dynamically allocating the space for node to tail pointer 
    temp->tail = (node*)malloc(sizeof(node));
    // the next pointer in tail will always be NULL
    temp->tail->next = NULL;
    // initially the head and tail point to same node this refers to empty
    temp->head = temp->tail;
    temp->size = 0; // initially the size will be 0

    return temp; // returning the temp pointer
}

bool isEmpty(iterators _iterators)
{
    /* when there is no data in head pointer it means it is empty */
    if(_iterators.head->data == NULL) return true;
    else return false;
}

unsigned getSize(iterators _iterators)
{
    // returning the value of size in iterators struct
    return _iterators.size;
}