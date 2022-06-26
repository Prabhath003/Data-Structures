#include "stack.h"

stack* initStack()
{
    stack* temp;
    /*
     * calling the function from NODES to create the pointer of iterators struct
     * assigning it temp variable
     */
    temp = (stack*)init();
    
    return temp; // returning the temp pointer
}

void push(stack* _stack, char *_data)
{
    // using the function from inplementation of list
    pushFront(_stack, _data);
}

char* pop(stack *_stack)
{
    /*
     * creating the temporary data variable to store the poping data 
     * and return it after deleting the node
     */
    char *temp_data;
    temp_data = (char*)malloc(data_length * sizeof(char));
    // using the implementation function from list
    strcpy(temp_data, getData(*_stack, 1));
    deleteData(_stack, temp_data);
    
    return temp_data;
}

char* top(stack _stack)
{
    if(isEmpty(_stack)) return "";
    /* we assume here top as the head pointer */
    else return _stack.head->data; // returing the data in the top node
}