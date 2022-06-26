#include "binarytree.h"

// checking to prevent defining more than once in a program
#ifndef BINARYTREE_DEF
#define BINARYTREE_DEF

binaryTree* initBinaryTree()
{
    /*
     * calling the function from TREENODE to create the pointer of tree struct
     * and returning it as binary tree pointer
     */
    return (binaryTree*)initTree();
}

void insertBinaryTreeNode(binaryTree* _binarytree, char* _data)
{
    // creating the node to be inserted
    treeNode *nodeToAdd = (treeNode*)malloc(sizeof(treeNode));
    nodeToAdd->data = (char*)malloc(data_length * sizeof(char));
    strcpy(nodeToAdd->data, _data);
    nodeToAdd->leftChild = NULL;
    nodeToAdd->RightChild = NULL;

    // if binary tree is empty the node to be inserted itself is the root
    if(isEmptyTree(*_binarytree))
    {
        _binarytree->root = nodeToAdd;
        _binarytree->totalNodes++;
        return;
    }

    else
    {
        // creating temporary variables to traverse to the place of insertion
        treeNode* temp;
        treeNode* start = _binarytree->root;
        while(start != NULL)
        {
            temp = start;
            // checking whether to go right
            if(strcmp(nodeToAdd->data, start->data) > 0)
                start = start->RightChild;
            // checking whether to go left
            else if(strcmp(nodeToAdd->data, start->data) < 0)
                start = start->leftChild;
            // stopping the insertion if the element already exits
            else
            {
                free(nodeToAdd);
                return;
            }
        }

        // assigning the parent pointer of the inserted node
        nodeToAdd->parentNode = temp;
        if(strcmp(nodeToAdd->data, temp->data) >= 0)
            temp->RightChild = nodeToAdd;
        else
            temp->leftChild = nodeToAdd;
        _binarytree->totalNodes++;
    }
}

/* deletion using recursion of nodes*/
treeNode* deleteBinaryRootData(treeNode* _root, char* _data)
{
    // stopping condition for recursion
    if(_root == NULL)
        return _root;
    // checking to recurse towards right child
    else if(strcmp(_data, _root->data) > 0)
        _root->RightChild = deleteBinaryRootData(_root->RightChild, _data);
    // checking to recurse towards leftchild
    else if(strcmp(_data, _root->data) < 0)
        _root->leftChild = deleteBinaryRootData(_root->leftChild, _data);
    else
    {
        // removing the leaf node
        if(_root->leftChild == NULL && _root->RightChild == NULL)
            return NULL;
        // removing one child node - left
        else if(_root->leftChild != NULL && _root->RightChild == NULL)
        {
            treeNode* temp = _root->leftChild;
            temp->parentNode = _root->parentNode;
            free(_root);
            return temp;
        }
        // removing one child node - right
        else if(_root->leftChild == NULL && _root->RightChild != NULL)
        {
            treeNode* temp = _root->RightChild;
            temp->parentNode = _root->parentNode;
            free(_root);
            return temp;
        }
        // removing two child node
        /*
         * this first copies the least element lexicographically from the right child into
         * the node to be deleted and then deletes the least element recusively
         */
        else if(_root->leftChild != NULL && _root->RightChild != NULL)
        {
            // searching for minimum element in the right child 
            treeNode* min = _root->RightChild;
            while(min->leftChild != NULL)
                min = min->leftChild;
            // copying the minimum element in present node
            strcpy(_root->data, min->data);
            // calling recursively to delete the mininum element from right child
            _root->RightChild = deleteBinaryRootData(_root->RightChild, min->data);
        }
    }
    // returing the recusively formed node
    return _root;
}

void deleteBinaryTreeData(binaryTree* _binarytree, char* _data)
{
    // finding whether it is present to delete
    if(findTreeNode(_binarytree->root, _data) != NULL)
    {
        // if present
        // then subtracting 1 from no. of nodes
        _binarytree->totalNodes--;
        // then deleting the node
        _binarytree->root = deleteBinaryRootData(_binarytree->root, _data);
    }
}

#endif