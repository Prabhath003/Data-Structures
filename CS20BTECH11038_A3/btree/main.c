// including libraries
#include <stdio.h>
#include "btree.h"

int main()
{
    // testing values
    int insertArr[60] = {17, 13, 2, 27, 48, 54, 39, 57, 60, 3, 23, 46, 16, 18, 49, 45, 33, 36, 55, 19, 47, 35, 7, 22, 4, 50, 9, 56, 37, 12, 11, 21, 31, 38, 29, 44, 8, 26, 25, 40, 6, 58, 51, 1, 15, 30, 52, 10, 28, 59, 53, 34, 43, 42, 24, 14, 32, 41, 5, 20};
    int searchArr[9] = {49, 27, 22, 38, 11, 55, 7, 35, 59};
    int deleteArr[9] = {13, 19, 24, 37, 43, 53, 18, 38, 58};

    // forming the storage
    reloadStorage();

    // initialising the binary search tree
    coordinate root = initTree();
    cost = 0;
    printf("\n\tINSERTION:\n");
    // doing the insetion of test array
    for(unsigned i = 0; i < 60; i++)
        insert(root, insertArr[i]);
    printf("The total time taken for insertion is %d\n", cost);
    coordinate location;
    printf("\n\tSEARCH:\n");
    for(unsigned i = 0; i < 9; i++)
    {
        // setting the cost variable to zero
        cost = 0;
        // searching each element of test array
        location = search(root, searchArr[i]);
        printf("The search time of %d is %d\n", searchArr[i], cost);
        printf("       - its location in secondary storage is (%d, %d)\n", location.n, location.x);
    }

    printf("\n\tDELETION:\n");
    for(unsigned i = 0; i < 9; i++)
    {
        // setting the cost variable to zero
        cost = 0;
        // deleting the elements according to test array
        delete(root, deleteArr[i]);
        printf("The deletion time of %d is %d\n", deleteArr[i], cost);
    }

    return 0;
}