#include "treenode.h"

// checking to prevent defining more than once in a program
#ifndef TREENODE_DEF
#define TREENODE_DEF

tree* initTree()
{
    tree* temp;
    // dynamically allocating the space to temp pointer
    temp = (tree*)malloc(sizeof(tree));
    // initializing the variables in temporary tree
    temp->totalNodes = 0;
    temp->root = NULL;

    // returning the dynamically created tree
    return temp;
}

bool isEmptyTree(tree _tree)
{
    /* if root points to NULL then tree is empty */
    if(_tree.root == NULL)
        return true;
    else 
        return false;
}

treeNode* findTreeNode(treeNode* _root, char* _data)
{
    /* returning NULL if required node isnt available
     * and if exists returns the pointer of the Node with
     * the data we required
     */
    if(_root == NULL)
        return _root;
    else if(strcmp(_data, _root->data) > 0)
        return findTreeNode(_root->RightChild, _data);
    else if(strcmp(_data, _root->data) < 0)
        return findTreeNode(_root->leftChild, _data);
    else
        return _root;
}

void printInorderTraverse(treeNode* _root)
{
    // stoppping statement for recursive call
    if(_root == NULL)
        return;
    // calling recursively towards the left child
    printInorderTraverse(_root->leftChild);
    // reading the present node
    printf("%s ", _root->data);
    // calling recursively towards the right child
    printInorderTraverse(_root->RightChild);
}

void printPreorderTraverse(treeNode* _root)
{
    // stopping statement for recursive call
    if(_root == NULL)
        return;
    // reading the present node
    printf("%s ", _root->data);
    // calling recursively towards the left child
    printPreorderTraverse(_root->leftChild);
    // calling recursively towards the rightChild
    printPreorderTraverse(_root->RightChild);
}

#endif