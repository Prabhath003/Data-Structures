// including the TREENODE
#include "treenode.h"

// checking to prevent declaring more than once in a program
#ifndef AVLTREE
#define AVLTREE

// defining type tree to type AVL tree
typedef tree avlTree;

// function initialize the AVL tree
avlTree* initAvlTree();
// function to insert an element into the AVL tree
void insertAvlTree(avlTree* _avlTree, char* _data);
// function to delete from the AVL tree using its pointer itself
void deleteAvlTreeData(avlTree* _avltree, char* _data);

#endif