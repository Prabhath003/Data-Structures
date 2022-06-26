#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// checking to prevent declaring more than once in a program
#ifndef TREENODE
#define TREENODE

// defining the length of string
#define data_length 30

typedef struct _treeNode treeNode;
// node to store the data
struct _treeNode
{
    treeNode* parentNode; // variable to store the pointer to parent Node
    char* data; // variable to store the data
    treeNode* leftChild; // variable to store pointer to left Child
    treeNode* RightChild; // variable to store pointer to right Child
    int height; // variable to store the height of Node
};

// struct to store the root and the total number of nodes
typedef struct
{
    treeNode* root; // variable to store the pointer to root
    unsigned totalNodes; // variable to store the total no. of nodes
} tree;

// function to initialize the tree pointer
tree* initTree();
// function to check whether the tree is empty
bool isEmptyTree(tree _tree);
// to find a element from the tree
treeNode* findTreeNode(treeNode* _root, char* _data);
// function to print the inorder traversal of tree
void printInorderTraverse(treeNode* _root);
// function to print the preoder traversal of tree
void printPreorderTraverse(treeNode* _root);

#endif