// including libraries
#include "bst.h"

// function to make the storage variables
void reloadStorage()
{
    // creating primary storage
    for(unsigned i = 0; i < 2 * t; i++)
    {
        primary1[i].key = -1;
        primary1[i].left.n = -1;
        primary1[i].left.x = -1;
        primary1[i].right.n = -1;
        primary1[i].right.x = -1;

        primary2[i].key = -1;
        primary2[i].left.n = -1;
        primary2[i].left.x = -1;
        primary2[i].right.n = -1;
        primary2[i].right.x = -1;
    }

    // creating secondary storage
    for(unsigned i = 0; i < N; i ++)
        for(unsigned j = 0; j < 2 * t; j++)
        {
            secondary[i][j].key = -1;
            secondary[i][j].left.n = -1;
            secondary[i][j].left.x = -1;
            secondary[i][j].right.n = -1;
            secondary[i][j].right.x = -1;
        }
    
    // initializing the cost variable
    cost = 0;
}

void writeDisk(int secondary_disk, int disk)
{
    // writing from primary disk 1
    if(disk == 1)
        for(unsigned i = 0; i < 2 * t; i++)
            secondary[secondary_disk][i] = primary1[i];
    // writing from primary disk 2
    else if(disk == 2)
        for(unsigned i = 0; i < 2 * t; i++)
            secondary[secondary_disk][i] = primary2[i];
    // incrementing the time disk access
    cost += disk_access_cost;
}

void readDisk(int secondary_disk, int disk)
{
    // reading to the disk 1
    if(disk == 1)
        for(unsigned i = 0; i < 2 * t; i++)
            primary1[i] = secondary[secondary_disk][i];
    // reading to the disk 2
    else if(disk == 2)
        for(unsigned i = 0; i < 2 * t; i++)
            primary2[i] = secondary[secondary_disk][i];
    // incrementing the time of copying 
    cost += disk_access_cost;
}

bool isEmptyTree(coordinate _root)
{
    // reading the root node in the primary memory
    readDisk(_root.n, 1);
    // incrementing the time
    cost += primary_key_access_cost;
    // checking if the root is empty
    if(primary1[_root.x].key == -1)
        return true;
    else
        return false;
}

coordinate initTree()
{
    // creating the root coordinate
    coordinate temp;
    temp.n = 0;
    temp.x = 0;
    // and returning
    return temp;
}

coordinate search(coordinate _root, int _data)
{
    // reading the node from secondary to start
    readDisk(_root.n, 1);
    cost += primary_key_access_cost;
    // checking if its empty
    if(primary1[_root.x].key == -1)
    {
        cost += primary_key_access_cost;
        return primary1[_root.x].left;
    }
    cell temp = primary1[_root.x];
    cost += primary_key_access_cost;
    while(true)
    {
        // if key exists returing its coordinate
        if(temp.key == _data)
            return _root;
        // going to the next right node
        else if(temp.key < _data)
        {
            if(_root.n == temp.right.n)
            {
                _root = temp.right;
                temp = primary1[temp.right.x];
                cost += primary_key_access_cost;
            }
            else
                return search(temp.right, _data);
        }
        // going to the next left node
        else
        {
            if(_root.n == temp.left.n)
            {
                _root = temp.left;
                temp = primary1[temp.left.x];
                cost += primary_key_access_cost;
            }
            else
                return search(temp.left, _data);
        }
    }
}

void insert(coordinate _root, int _data)
{
    // readint he root node into the primary memory
    readDisk(_root.n, 1);
    // checking if node if empty
    if(primary1[_root.x].key == -1)
    {
        // if empty then inserting the key here
        primary1[_root.x].key = _data;
        cost += 2 * primary_key_access_cost;
        // and then writing back in the secondary
        writeDisk(_root.n, 1);
        return; 
    }
    
    /* if root is not empty trying to find a free cell in the secondary to copy our
     * data there link to the tree
     */
    int freedisk = _root.n;
    int pos = -1;
    while(true)
    {
        readDisk(freedisk, 2);
        for(int i = 0; i < 2 * t; i++)
        {
            if(primary2[i].key == -1)
            {
                cost += primary_key_access_cost;
                pos = i;
                break;
            }
        }
        if(pos != -1)
            break;
        freedisk = (freedisk + 1) % N;
    }
    // copying the data into the free cell
    primary2[pos].key = _data;
    cost += primary_key_access_cost;
    writeDisk(freedisk, 2);
    if(freedisk == _root.n)
        readDisk(_root.n, 1);
    // now linking this cell to  our tree
    while(true)
    {
        cost += primary_key_access_cost;
        // stopping the insertion if the element already exits
        if(primary1[_root.x].key == _data)
            return;
        // if value of root is less than data then traversing towards right
        else if(primary1[_root.x].key < _data)
        {
            int nextDisk = primary1[_root.x].right.n;
            cost += 2 * primary_key_access_cost;
            // finding the right cell and copying to primary
            if(_root.n == nextDisk)
            {
                _root = primary1[_root.x].right;
                cost += primary_key_access_cost;
            }
            // if right node is empty
            else if(nextDisk == -1)
            {
                primary1[_root.x].right.n = freedisk;
                primary1[_root.x].right.x = pos;
                cost += 2 * primary_key_access_cost;
                writeDisk(_root.n, 1);
                break;
            }
            else
            {
                int n = _root.n;
                _root = primary1[_root.x].right;
                cost += primary_key_access_cost;
                if(n != _root.n)
                    readDisk(_root.n, 1);
            }
        }
        // if value of root is less than data then traversing towards left
        else
        {
            int nextDisk = primary1[_root.x].left.n;
            cost += 2 * primary_key_access_cost;
            // finding the left cell and copying to primary
            if(_root.n == nextDisk)
            {
                _root = primary1[_root.x].left;
                cost += primary_key_access_cost;
            }
            // if left node is empty
            else if(nextDisk == -1)
            {
                primary1[_root.x].left.n = freedisk;
                primary1[_root.x].left.x = pos;
                cost += 2 * primary_key_access_cost;
                writeDisk(_root.n, 1);
                break;
            }
            else
            {
                int n = _root.n;
                _root = primary1[_root.x].left;
                cost += primary_key_access_cost;
                if(n != _root.n)
                    readDisk(_root.n, 1);
            }
        }
    }
}

coordinate delete(coordinate _root, int _data)
{
    // if root is not pointing to anything then stopping the deletion
    if(_root.n == -1)
        return _root;
    // if not empty reading the root node into the primary
    readDisk(_root.n, 1);
    int key = primary1[_root.x].key;
    cost += primary_key_access_cost;
    // if coordinate pointing to a empty cell then just stopping the process
    if(key == -1)
        return _root;
    // checking to recurse towards right child
    else if(key < _data)
    {
        coordinate result = delete(primary1[_root.x].right, _data);
        if(result.n != _root.n)
            readDisk(_root.n, 1);
        primary1[_root.x].right = result;
        cost += 2 * primary_key_access_cost;
        writeDisk(_root.n, 1);
        return _root;
    }
    // checking to recurse towards leftchild
    else if(key > _data)
    {
        coordinate result = delete(primary1[_root.x].left, _data);
        if(result.n != _root.n)
            readDisk(_root.n, 1);
        primary1[_root.x].left = result;
        cost += 2 * primary_key_access_cost;
        writeDisk(_root.n, 1);
        return _root;
    }
    else
    {
        int rightDisk = primary1[_root.x].right.n, leftDisk = primary1[_root.x].left.n;
        cost += 2 * primary_key_access_cost;
        // removing the leaf node
        if(rightDisk == -1 && leftDisk == -1)
        {
            primary1[_root.x].key == -1;
            cost += primary_key_access_cost;
            writeDisk(_root.n, 1);
            coordinate result;
            result.n = -1;
            result.x = -1;
            return result;
        }
        // removing one child node - left
        else if(rightDisk == -1 && leftDisk != -1)
        {
            coordinate result = primary1[_root.x].left;
            primary1[_root.x].key = -1;
            primary1[_root.x].left.n = -1;
            primary1[_root.x].left.x = -1;
            cost += 4 * primary_key_access_cost;
            writeDisk(_root.n, 1);
            return result;
        }
        // removing one child node - right
        else if(rightDisk != -1 && leftDisk == -1)
        {
            coordinate result = primary1[_root.x].right;
            primary1[_root.x].key = -1;
            primary1[_root.x].right.n = -1;
            primary1[_root.x].right.x = -1;
            cost += 4 * primary_key_access_cost;
            writeDisk(_root.n, 1);
            return result;
        }
        // removing two child node
        /*
         * this first copies the least element from the right child into
         * the cell to be deleted and then deletes the least element recusively
         */
        else if(rightDisk != -1 && leftDisk != -1)
        {
            // searching for minimum element in the right child 
            coordinate min = primary1[_root.x].right;
            cost += primary_key_access_cost;
            readDisk(min.n, 2);
            while(primary2[min.x].left.n != -1)
            {
                int n = min.n;
                min = primary2[min.x].left;
                cost += 2 * primary_key_access_cost;
                if(min.n != n)
                    readDisk(min.n, 2);
            }
            // copying the minimum element in present node
            primary1[_root.x].key = primary2[min.x].key;
            cost += 2 * primary_key_access_cost;
            writeDisk(_root.n, 1);
            // calling recursively to delete the mininum element from right child
            min = delete(primary1[_root.x].right, primary1[_root.x].key);
            cost += 2 * primary_key_access_cost;
            if(_root.n != min.n)
                readDisk(_root.n, 1);
            primary1[_root.x].right = min;
            cost += primary_key_access_cost;
            writeDisk(_root.n, 1);
            return _root;
        }
    }
}