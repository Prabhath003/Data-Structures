// inclding the TREENODE
#include "treenode.h"

// checking to prevent declaring more than once in a program
#ifndef BINARYTREE
#define BINARYTREE

// defining type tree to type binary tree
typedef tree binaryTree;

// function initialize the binary tree
binaryTree* initBinaryTree();
// function to insert an element into the Binarytree
void insertBinaryTreeNode(binaryTree* _binarytree, char* _data);
// function to delete from the Binary tree using its pointer itself
void deleteBinaryTreeData(binaryTree* _binarytree, char* _data);

#endif