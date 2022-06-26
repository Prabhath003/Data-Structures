#include "list.h"

#ifndef QUEUE
#define QUEUE

// modifying list to queue
typedef list queue;

/*
 * We are adding a data at the tail pointer
 * and removing(or accesing) from head pointer
 */

// function to create queue and return its pointer
queue* initQueue();
// function to add given data to queue at tail
void enQueue(queue* _queue, char *_data);
// function to get the data from queue at head
char* deQueue(queue* _queue);

#endif