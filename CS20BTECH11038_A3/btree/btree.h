// including the libraries
#include <stdbool.h>
#include <stdlib.h>

// defining constants
#define t 10
#define N 10
#define primary_key_access_cost 1
#define disk_access_cost 10

// creating struct to store coordinates
typedef struct
{
    int n;
    int x;
} coordinate;

// creating struct to store the key value and prev, next coordinate
typedef struct
{
    coordinate leftchild;
    int key;
    coordinate next;
} cell;

// declaring primary and secondary storage
cell primary1[2 * t];
cell primary2[2 * t];
cell secondary[N][2 * t];
// declaring the variable to calculate time
int cost;

// function to store 
void reloadStorage();

// function to write a disk into primary 
void writeDisk(int secondary_disk, int disk);
// function to read a disk into primary
void readDisk(int secondary_disk, int _disk);

// function to check whether the tree is empty
bool isEmptyTree(coordinate _root);
// function to init tree
coordinate initTree();
// function to search a element in the tree
coordinate search(coordinate _root, int _data);
// function to insert a element
void insert(coordinate _root, int _data);
// function to delete a element
void delete(coordinate _root, int _data);