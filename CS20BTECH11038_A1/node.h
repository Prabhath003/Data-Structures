#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// checking to prevent declaring more than once in a program
#ifndef NODES
#define NODES

// defining the length of string
#define data_length 30

// node to store the data and the pointer of next node
struct _node
{
    char *data; // char variable to store data
    struct _node* next; // pointer to point to next node;
};
typedef struct _node node;

// struct storing iterating pointers to traverse through the data structure
typedef struct
{
    node *head; // head pointer
    node *tail; // tail pointer
    unsigned size; // variable to store the length of data structure
} iterators;

// function to create the the data structure and return its pointer
iterators* init();
// function to check whether the given data structure is empty
bool isEmpty(iterators _iterators);
// function to return the length of data structure
unsigned getSize(iterators _iterators);

#endif