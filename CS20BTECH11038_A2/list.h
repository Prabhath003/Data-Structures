// including the NODES
#include "node.h"

#ifndef LIST
#define LIST

// now we will call the struct storing the pointers to traverse as list
typedef iterators list;

// function to create list
list* initList();
//function to push from front into given list
void pushFront(list* _list, char *_data);
// function to push from back into given list
void pushBack(list* _list, char *_data);
// function to find a given data whether it is in the list and return its position
unsigned find(list _list, char *_data);
// function to get the data from the given position
char* getData(list _list, unsigned position);
// function to delete the given data from the list
void deleteData(list* _list, char *_data);

#endif