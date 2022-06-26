#include "list.h"

#ifndef STACK
#define STACK

// modifying list to use as stack
typedef list stack;

/*
 * we are pusing and poping at same head pointer
 */

// function to create stack and return its pointer
stack* initStack();
// function to push an element into the stack
void push(stack* _stack, char *_data);
// function to get data and remove the top element from the stack 
char* pop(stack *_stack);
// function to get data present top element in the stack
char* top(stack _stack);

#endif