#include "avlTree.h"

// checking to prevent defining more than once in a program
#ifndef AVLTREE_DEF
#define AVLTREE_DEF

avlTree* initAvlTree()
{
    /*
     * calling the function from TREENODE to create the pointer of tree struct
     * and returning it as AVL tree pointer
     */
    return (avlTree*)initTree();
}

// function to calculate maximum of two numbers
int max(int num1, int num2)
{
    if(num1 > num2)
        return num1;
    return num2;
}

// function to return the height of a given node
int height(treeNode *_node)
{
    // returing -1 as height if node is NULL
    if (_node == NULL)
        return -1;
    return _node->height;
}

// function to rotate rightwards at given node
treeNode* rightRotate(treeNode *_node)
{
    // creating temporary nodes to rotate
    treeNode *leftNew = _node->leftChild;
    treeNode *left_RightNode = leftNew->RightChild;
 
    // swaping to rotate the nodes
    leftNew->RightChild = _node;
    leftNew->RightChild->parentNode = leftNew;
    _node->leftChild = left_RightNode;
    if(leftNew->RightChild->leftChild != NULL)
        leftNew->RightChild->leftChild->parentNode = leftNew->RightChild;
 
    // Updating the heights after rotation
    _node->height = max(height(_node->leftChild),
                    height(_node->RightChild)) + 1;
    leftNew->height = max(height(leftNew->leftChild),
                    height(leftNew->RightChild)) + 1;
 
    // returning the rotated node
    return leftNew;
}

// function to leftwards at given node
treeNode* leftRotate(treeNode *_node)
{
    // creating temporary nodes to rotate
    treeNode *rightNew = _node->RightChild;
    treeNode *right_LeftNode = rightNew->leftChild;
 
    // swaping to rotate the nodes
    rightNew->leftChild = _node;
    rightNew->leftChild->parentNode = rightNew;
    _node->RightChild = right_LeftNode;
    if(rightNew->leftChild->RightChild != NULL)
        rightNew->leftChild->RightChild->parentNode = rightNew->leftChild; 
 
    // Updating the heights after rotation
    rightNew->leftChild->height = max(height(rightNew->leftChild->leftChild),   
                    height(rightNew->leftChild->RightChild)) + 1;
    rightNew->height = max(height(rightNew->leftChild),
                    height(rightNew->RightChild)) + 1;
 
    // returning the rotated node
    return rightNew;
}

void insertAvlTree(avlTree* _avlTree, char* _data)
{
    // creating the node to be inserted
    treeNode *nodeToAdd = (treeNode*)malloc(sizeof(treeNode));
    nodeToAdd->data = (char*)malloc(data_length * sizeof(char));
    strcpy(nodeToAdd->data, _data);
    nodeToAdd->height = 0;
    nodeToAdd->leftChild = NULL;
    nodeToAdd->RightChild = NULL;

    // if Avl tree empty then the node to be inserted itself is the root
    if(isEmptyTree(*_avlTree))
    {
        _avlTree->root = nodeToAdd;
        _avlTree->totalNodes++;
        return;
    }
    else
    {
        // creating the temporary variable to traverse to place of insertion
        treeNode* temp;
        treeNode* start = _avlTree->root;
        while(start != NULL)
        {
            temp = start;
            // checking whether to go right
            if(strcmp(nodeToAdd->data, start->data) > 0)
            {
                // updating the height of present node before going to right
                if(height(start->RightChild) + 1 > height(start->leftChild)) 
                    temp->height++;
                start = start->RightChild;
            }
            // checking whether to go left
            else if(strcmp(nodeToAdd->data, start->data) < 0)
            {
                // updating the height of present node before going to the left
                if(height(start->leftChild) + 1 > height(start->RightChild)) 
                    temp->height++;
                start = start->leftChild;
            }
            // stopping the insertion if the element already exists
            else
            {
                free(nodeToAdd);
                return;
            }
        }

        // assignin the parent pointer to the inserted node
        nodeToAdd->parentNode = temp;
        if(strcmp(nodeToAdd->data, temp->data) >= 0)
            temp->RightChild = nodeToAdd;
        else
            temp->leftChild = nodeToAdd;
        _avlTree->totalNodes++;

        // creating temporary variables to traverse back and check the balancing condition
        treeNode *first, *second, *third;
        first = nodeToAdd->parentNode->parentNode;
        second = nodeToAdd->parentNode;
        third = nodeToAdd;
        while(first != NULL) // checking whether root node has come
        {
            // checking the condition of balancing
            if(abs(height(first->leftChild) - height(first->RightChild)) <= 1)
            {
                // if balanced then moving one node upwards
                third = second;
                second = first;
                first = first->parentNode;
            }
            else
            {
                // creating temporary node to store parentNode of first temporarly
                treeNode* temp = first->parentNode;
                // checking for right-right case of insertion
                if((strcmp(first->data, second->data) < 0) && (strcmp(second->data, third->data) < 0))
                    // doing left rotate at first
                    first = leftRotate(first);
                // checking for left-left case of insertion
                else if(strcmp(first->data, second->data) > 0 && strcmp(second->data, third->data) > 0)
                    // doing right rotation at first
                    first = rightRotate(first);
                // checking for left-right case of insertion
                else if(strcmp(first->data, second->data) > 0 && strcmp(second->data, third->data) < 0)
                {
                    // doing left rotation at second node
                    first->leftChild = leftRotate(second);
                    first->leftChild->parentNode = first;
                    // doing right rotation at first node
                    first = rightRotate(first);
                }
                // checking for right-left case of insertion
                else if(strcmp(first->data, second->data) < 0 && strcmp(second->data, third->data) > 0)
                {
                    // doing right rotation at second node
                    first->RightChild = rightRotate(second);
                    first->RightChild->parentNode = first;
                    // doing left roation at first node
                    first = leftRotate(first);
                }
                /* 
                 * if parent node temporarly stored before is NULL
                 * then first will itself become the root
                 */
                if(temp == NULL)
                {
                    _avlTree->root = first;
                    _avlTree->root->parentNode = NULL;
                }
                else
                {
                    /* connecting the first node to its parent node */
                    first->parentNode = temp;
                    if(strcmp(first->data, temp->data) > 0)
                        temp->RightChild = first;
                    else
                        temp->leftChild = first;
                    /*
                     * updating the heights of the above nodes of first
                     * after rotation
                     */
                    while(temp != NULL)
                    {
                        temp->height--;
                        temp = temp->parentNode;
                    }
                }
                return;
            }
        }
    }
}

// recursive function to delete the given data
treeNode* deleteAvlRootData(treeNode* _root, char* _data)
{
    // stopping condition for recursion
    if(_root == NULL)
        return _root;
    // checking to recurse towards the right child
    else if(strcmp(_data, _root->data) > 0)
        _root->RightChild = deleteAvlRootData(_root->RightChild, _data);
    // checking to recurse towards the left child
    else if(strcmp(_data, _root->data) < 0)
        _root->leftChild = deleteAvlRootData(_root->leftChild, _data);
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
            _root = temp;
        }
        // removing one child node - right
        else if(_root->leftChild == NULL && _root->RightChild != NULL)
        {
            treeNode* temp = _root->RightChild;
            temp->parentNode = _root->parentNode;
            _root = temp;
        }
        // removing two child node
        /*
         * this first copies the least element lexicographically from the right child into
         * the node to be deleted and then deletes the least element recusively
         */
        else if(_root->leftChild != NULL && _root->RightChild != NULL)
        {
            treeNode* min = _root->RightChild;
            while(min->leftChild != NULL)
                min = min->leftChild;
            strcpy(_root->data, min->data);
            _root->RightChild = deleteAvlRootData(_root->RightChild, min->data);
        }
    }
    // updating the height of present node
    _root->height = max(height(_root->leftChild), height(_root->RightChild)) + 1;

    // checking the condition of balancing for present node
    if(abs(height(_root->leftChild) - height(_root->RightChild)) > 1)
    {
        // if not balanced
        // checking the orientation of present node and its child
        // when right node of present node has greater height
        if(height(_root->leftChild) - height(_root->RightChild) < -1)
        {
            // when right node of right node of present node has greater height
            if(height(_root->RightChild->leftChild) - height(_root->RightChild->RightChild) <= 0)
            {
                // doing left rotation on present node
                treeNode* temp = leftRotate(_root);
                temp->parentNode = _root->parentNode;
                _root = temp;
            }
            // when left node of right node of present node has greater height
            else
            {
                // doing right rotate on right child of present node
                _root->RightChild = rightRotate(_root->RightChild);
                // then doing left rotate on present node
                treeNode* temp = leftRotate(_root);
                temp->parentNode = _root->parentNode;
                _root = temp;
            }
        }
        // when left node of present has greater height
        else if(height(_root->leftChild) - height(_root->RightChild) > 1)
        {
            // when right node of left node of present node has greate height
            if(height(_root->leftChild->leftChild) - height(_root->leftChild->RightChild) <= 0)
            {
                // doing left rotation on left child of present node
                _root->leftChild = leftRotate(_root->leftChild);
                // then doing left rotation on present node
                treeNode* temp = rightRotate(_root);
                temp->parentNode = _root->parentNode;
                _root = temp;
            }
            // when left node of left node of present node has greater height
            else
            {
                // doing right rotation on present node
                treeNode* temp = rightRotate(_root);
                temp->parentNode = _root->parentNode;
                _root = temp;
            }
        }
    }

    // returning the node
    return _root;
}

void deleteAvlTreeData(avlTree* _avltree, char* _data)
{
    // finding whether it is present to delete
    if(findTreeNode(_avltree->root, _data) != NULL)
    {
        // if present
        // then subtracting 1 from no. of nodes
        _avltree->totalNodes--;
        // then deleting the node
        _avltree->root = deleteAvlRootData(_avltree->root, _data);
    }
}

#endif