#include "list.h"

list* initList()
{
    list* temp;
    /*
     * calling the function from NODES to create the pointer of iterators struct
     * assigning it temp variable
     */ 
    temp = (list*)init();

    return temp; // returning the temp pointer
}

/* we assume the head as front and tail as back */
void pushFront(list* _list, char *_data)
{
    if(isEmpty(*_list))
    {
        /*
         * already head pointer and tail pointer are pointing to a node
         * dynamically allocating the space to the data variable in head pointer
         * copying the given data into it
         */
        _list->head->data = (char*)malloc(data_length * sizeof(char));
        strcpy(_list->head->data, _data);
        _list->size++;
    }
    else
    {
        /*
         * if not empty then just creating a new node and adding it at head
         */
        node *tmp;
        tmp = (node*)malloc(sizeof(node));
        tmp->data = (char*)malloc(data_length * sizeof(char));
        strcpy(tmp->data, _data);
        tmp->next = _list->head;
        _list->head = tmp;
        _list->size++;
    }
}

void pushBack(list* _list, char* _data)
{
    if(isEmpty(*_list))
    {
        /*
         * already head pointer and tail pointer are pointing to a node
         * dynamically allocating the space to the data variable in head pointer
         * copying the given data into it
         */
        _list->tail->data = (char*)malloc(data_length * sizeof(char));
        strcpy(_list->tail->data, _data);
        _list->size++;
    }
    else
    {
        /*
         * if not empty then just creating a new node and adding it at tail
         */
        node *tmp;
        tmp = (node*)malloc(sizeof(node));
        tmp->data = (char*)malloc(data_length * sizeof(char));
        strcpy(tmp->data, _data);
        tmp->next = NULL;
        _list->tail->next = tmp;
        _list->tail = tmp;
        _list->size++;
    }
}

unsigned find(list _list, char *_data)
{
    /* finding the given element by iterating the given list 
     * by traversing from head to tail
     */
    if(isEmpty(_list)) return -1; // if empty returns -1
    unsigned j = 0;
    for(node *i = _list.head; i != NULL; i = i->next)
    {
        if(strcmp(i->data, _data) == 0) return j + 1; // if found returns the position of that data
        // positions start from 1
        j++;
    }
    return -1; // if not found returns -1
}

// positions start from 1
char* getData(list _list, unsigned position)
{
    // checking for valid position
    if(position > _list.size)
    {
        printf("Error: The position given is out of bound with respect to size of given list\n");
        return "";
    }
    // traversing the list from head to tail and returning the data at that position
    unsigned j = 0;
    for(node *i = _list.head; i != NULL; i = i->next)
    {
        if(j+1 == position)
            return i->data;
        j++;
    }
    return ""; 
}

void deleteData(list* _list, char *_data)
{
    if(_data == NULL) return;
    unsigned position = find(*_list, _data); // finding the position of that data using find function
    if(position == -1) return; // if find function returns -1 
    if(position == 1) // if position is head
    {
        if(_list->head == _list->tail) // head and tail pointer are same
        {
            // the data pointer should be made NULL so traversing pointers aren't disturbed
            _list->head->data = NULL;
            _list->size--;
        }
        else
        {
            // just changing the head pointer to next
            _list->head = _list->head->next;
            _list->size--;
        }
        return;
    }
    unsigned j = 0;
    /*
     * if in the middle then just making the next pointer of previous
     * node to point to the next node
     */
    for(node *i = _list->head; i != NULL; i = i->next)
    {
        if(j + 1 == position - 1)
        {
            i->next = i->next->next;
            /* if node pointed by tail is to be deleted then the tail pointer should be
             * changed to point to previous node
             */
            if(j == _list->size - 2) 
                _list->tail = i;
            _list->size--;
            return;
        }
        j++;
    }
}