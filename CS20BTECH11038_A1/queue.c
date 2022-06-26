#include "queue.h"

queue* initQueue()
{
    queue* temp;
    /*
     * calling the function from NODES to create the pointer of iterators struct
     * assigning it temp variable
     */ 
    temp = (queue*)init();

    return temp; // returning the temp pointer
}

void enQueue(queue* _queue, char *_data)
{
    // using the function from inplementation of list
    pushBack(_queue,_data);
}

char* deQueue(queue* _queue)
{
    /*
     * creating the temporary data variable to store the poping data 
     * and return it after deleting the node
     */
    char *temp_data;
    temp_data = (char*)malloc(data_length * sizeof(char));
    // using the implementation function from list
    strcpy(temp_data, getData(*_queue, 1));
    deleteData(_queue, temp_data);
    
    return temp_data;
}