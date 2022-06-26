// including libraries
#include "btree.h"

// function to make the storage variables
void reloadStorage()
{
    // creating primary storage
    for(unsigned i = 0; i < 2 * t; i++)
    {
        primary1[i].key = -1;
        primary1[i].leftchild.n = -1;
        primary1[i].leftchild.x = -1;
        primary1[i].next.n = -1;
        primary1[i].next.x = -1;

        primary2[i].key = -1;
        primary2[i].leftchild.n = -1;
        primary2[i].leftchild.x = -1;
        primary2[i].next.n = -1;
        primary2[i].next.x = -1;
    }

    // creating secondary storage
    for(unsigned i = 0; i < N; i ++)
        for(unsigned j = 0; j < 2 * t; j++)
        {
            secondary[i][j].key = -1;
            secondary[i][j].leftchild.n = -1;
            secondary[i][j].leftchild.x = -1;
            secondary[i][j].next.n = -1;
            secondary[i][j].next.x = -1;
        }
    // initializing the cost variable
    cost = 0;
}

// function to reload a cell to empty
void reloadcell(int i, int disk)
{
    // reloading cell in primary1
    if(disk == 1)
    {
        primary1[i].key = -1;
        primary1[i].leftchild.n = -1;
        primary1[i].leftchild.x = -1;
        primary1[i].next.n = -1;
        primary1[i].next.x = -1;
    }
    // reloading cell in primary2
    else if(disk == 2)
    {
        primary2[i].key = -1;
        primary2[i].leftchild.n = -1;
        primary2[i].leftchild.x = -1;
        primary2[i].next.n = -1;
        primary2[i].next.x = -1;
    }
    // incrementing the time variable 
    cost += 5 * primary_key_access_cost;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////* FUNCTION TO CALCULATE LENGTH *///////

// function to length of the primary memory
int getlength(int disk)
{
    int len = 0;
    cost += primary_key_access_cost;
    // caluculating the length of primary1
    if(disk == 1)
        while(primary1[len].key != -1)
        {
            cost += primary_key_access_cost;
            len++;
        }
    // calculating the length of primary2
    else
        while(primary2[len].key != -1)
        {
            cost += primary_key_access_cost;
            len++;
        }

    // returning the length
    return len;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////* FUNCTIONS TO ACCESS THE SECONDARY MEMORY *///////

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////* FUNCTIONS TO INITIALIZE, SEARCH, AND CHECK EMPTINESS IN B-TREE(t = 10) *//////

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
    // if root isnt pointing to anything returnig the 
    if(_root.n == -1)
        return _root;
    readDisk(_root.n, 1);
    int i = 0;
    int key = primary1[i].key;
    cost += primary_key_access_cost;
    // travelling through the node to find just greater than or equal to data
    while (key != -1 && _data > key)
    {
        i++;
        key = primary1[i].key;
        cost += primary_key_access_cost;
    }
    // if key exists returing its coordinate
    if (key == _data)
    {
        _root.n = _root.n;
        _root.x = i;
        return _root;
    }
    // going to the next left node
    else if(_data < key)
    {
        cost += primary_key_access_cost;
        return search(primary1[i].leftchild, _data);
    }
    // going to the next right node
    else
    {
        cost += primary_key_access_cost;
        return search(primary1[i - 1].next, _data);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
        //////* FUNCTIONS TO INSERT A KEY INTO TREE *///////

void insertNonFull(coordinate _root, int _data)
{
    cost += primary_key_access_cost;
    /*
     * checking if the the coordinate points to a leaf
     */
    if(primary1[_root.x].leftchild.n == -1)
    {
        int len = getlength(1);
        primary1[len].key = _data;
        cost += primary_key_access_cost;
        // if node is not empty
        if(len != 0)
        {
            primary1[len - 1].next.n = _root.n;
            primary1[len - 1].next.x = len;
            cost += 2 * primary_key_access_cost;
        }
        // sorting the keys in node
        for(unsigned i = len; i >= 1; i--)
        {
            cost += 2 * primary_key_access_cost;
            if(primary1[i].key < primary1[i - 1].key)
            {
                int temp = primary1[i].key;
                primary1[i].key = primary1[i - 1].key;
                primary1[i - 1].key = temp;
                cost += 3 * primary_key_access_cost;
            }
            else
                break;
        }
        // writing back the node
        writeDisk(_root.n, 1);
    }
    /*
     * if not leaf
     */
    else
    {
        // creating the coordinate to store next child which to be traversed
        coordinate nextChild;
        int len = getlength(1);
        int pos = -1;
        int child;
        /* finding a positon of cell which contains just greater
         * element than data to be inserted
         */
        for(unsigned i = 0; i < len; i++)
        {
            cost += primary_key_access_cost;
            if(primary1[i].key > _data)
            {
                pos = i;
                nextChild = primary1[pos].leftchild;
                child = nextChild.n;
                readDisk(child, 2);
                cost += primary_key_access_cost;
                break;
            }
        }
        /* if all elements are less than the data to be inserted
         * setting the last element for next pointer
         */
        if(pos == -1)
        {
            pos = len - 1;
            nextChild = primary1[pos].next;
            child = nextChild.n;
            cost += primary_key_access_cost;
            readDisk(child, 2);
        }
        int lenC = getlength(2);
        // if length of child node which we found is equal to 2t - 1
        if(lenC == 2 * t - 1)
        {
            /*
             * first we get the empty node from secondary and copy half of the key
             * and rewrite it into the secondary, the new parent key will be copied
             * in their parent node, and the previous node will be there with the remaining
             * and the splitting done
             */
            cost += primary_key_access_cost;
            // splitting the child

            // shifting the keys to add the new key from the child
            // pos is the last cell
            if(primary1[pos].key < _data)
            {
                pos++;
                primary1[pos].key = primary2[t - 1].key;
                primary1[pos].leftchild = primary1[pos - 1].next;
                primary1[pos - 1].next.n = _root.n;
                primary1[pos - 1].next.x = pos;
                cost += 6 * primary_key_access_cost;
            }
            // pos if not the last cell
            else
            {
                for(unsigned i = len; i > pos; i--)
                {
                    if(i != len)
                    {
                        cost += primary_key_access_cost;
                        primary1[i - 1].next.x++;
                    }
                    primary1[i] = primary1[i - 1];
                    cost += 2 * primary_key_access_cost;
                }
                primary1[pos].key = primary2[t - 1].key;
                primary1[pos].next.x = pos + 1;
                primary1[pos].next.n = _root.n;
                cost += 3 * primary_key_access_cost;
            }
            primary2[t - 2].next = primary2[t - 1].leftchild;
            cost += 2 * primary_key_access_cost;
            reloadcell(t - 1, 2);
            for(unsigned i = t; i < 2 * t - 2; i++)
            {
                cost += primary_key_access_cost;
                primary2[i].next.x = i - t + 1;
            }
            writeDisk(primary1[pos].leftchild.n, 2);
            cost += primary_key_access_cost;
            // finding a free disk available in the secondary
            int freedisk = (_root.n + 1) % N;
            while(true)
            {
                readDisk(freedisk, 2);
                if(primary2[0].key == -1)
                    break;
                freedisk = (freedisk + 1) % N;
            }

            cost += primary_key_access_cost;
            if(primary1[pos].next.n != -1)
            {
                primary1[pos + 1].leftchild.n = freedisk;
                primary1[pos + 1].leftchild.x = 0;
                cost += 3 * primary_key_access_cost;
                if(primary1[pos].key < _data)
                {
                    nextChild = primary1[pos + 1].leftchild;
                    cost += primary_key_access_cost;
                }
                else
                {
                    cost += primary_key_access_cost;
                    nextChild = primary1[pos].leftchild;
                }
            }
            else
            {
                primary1[pos].next.n = freedisk;
                primary1[pos].next.x = 0;
                cost += 3 * primary_key_access_cost;
                if(primary1[pos].key < _data)
                {
                    cost += primary_key_access_cost;
                    nextChild = primary1[pos].next;
                }
                else
                {
                    cost += primary_key_access_cost;
                    nextChild = primary1[pos].leftchild;
                }
            }
            writeDisk(_root.n, 1);
            readDisk(child, 1);
            for(unsigned i = t; i < 2 * t - 1; i++)
            {
                primary2[i - t] = primary1[i];
                cost += primary_key_access_cost;
                if(i != 2 * t - 2)
                {
                    primary2[i - t].next.n = freedisk;
                    cost += primary_key_access_cost;
                }

                reloadcell(i, 1);
            }
            writeDisk(child, 1);
            writeDisk(freedisk, 2);
        }
        // after splitting if done, then recursively the function to insert the data
        readDisk(nextChild.n, 1);
        insertNonFull(nextChild, _data);
    }
}

void insert(coordinate _root, int _data)
{
    readDisk(_root.n, 1);
    int len = getlength(1);

    // if the root node is full
    if (len == 2 * t - 1) // root r is full
    {
        /*
         * we will first find an empty node from secondary copy half into this and other half will be
         * root itself then rewriting it back into secondary storage and find other empty node
         * and writing other keys into this rewriting it back the root key will be in the 
         * root node
         */ 
        // then we will search for free node and then split
        // finding a free disk
        int freedisk = 1;
        while(true)
        {
            readDisk(freedisk, 2);
            cost += primary_key_access_cost;
            if(primary2[0].key == -1)
                break;
            freedisk = (freedisk + 1) % N;
        }
        for(unsigned i = t; i < 2 * t - 1; i++)
        {
            primary2[i - t] = primary1[i];
            cost += primary_key_access_cost;
            if(i != 2 * t - 2)
            {
                primary2[i - t].next.n = freedisk;
                primary2[i - t].next.x = i - t + 1;
                cost += 2 * primary_key_access_cost;
            }
            reloadcell(i, 1);
        }
        primary1[t - 1].next.n = freedisk;
        primary1[t - 1].next.x = 0;
        cost += 2 * primary_key_access_cost;
        writeDisk(freedisk, 2);
        // finding a free disk
        freedisk = (freedisk + 1) % N;
        while(true)
        {
            readDisk(freedisk, 2);
            if(primary2[0].key == -1)
                break;
            freedisk = (freedisk + 1) % N;
        }
        for(unsigned i = 0; i < t - 1; i++)
        {
            primary2[i] = primary1[i];
            primary2[i].next.n = freedisk;
            cost += 2 * primary_key_access_cost;
            reloadcell(i, 1);
        }
        primary2[t - 2].next = primary1[t - 1].leftchild;
        primary1[t - 1].leftchild.n = freedisk;
        primary1[t - 1].leftchild.x = 0;
        primary1[0] = primary1[t - 1];
        cost += 6 * primary_key_access_cost;
        reloadcell(t - 1, 1);
        writeDisk(_root.n, 1);
        writeDisk(freedisk, 2);
        // after splitting calling the nonfullinsert function
        insertNonFull(_root, _data);
    }
    else
        insertNonFull(_root, _data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////* FUNCTIONS TO MERGE, GET PREDESSECOR, SUCCESSOR AT A GIVEN KEY IN A NODE *//////

// A function to get predecessor of given coordinate
int getPred(coordinate _root)
{
    // initializing with left child of root
    _root = primary1[_root.x].leftchild;
    cost += primary_key_access_cost;
	readDisk(_root.n, 2);
    int len = getlength(2);
    cost += primary_key_access_cost;
    // Keep moving to the right most node until we reach a leaf
	while(primary2[len - 1].next.n != -1)
    {
        cost += primary_key_access_cost;
        _root = primary2[len - 1].next;
        cost += primary_key_access_cost;
	    readDisk(_root.n, 2);
		len = getlength(2);
    }

	// Return the last key of the leaf
    cost += primary_key_access_cost;
	return primary2[len - 1].key;
}

// A function to get successor of given coordinate
int getSucc(coordinate _root)
{
    cost += primary_key_access_cost;
    // initializing with right child of given root
    if(primary1[_root.x + 1].key == -1)
    {
        _root = primary1[_root.x].next;
        cost += primary_key_access_cost;
    }
    else
    {
        _root = primary1[_root.x + 1].leftchild;
        cost += primary_key_access_cost;
    }
	readDisk(_root.n, 2);
    // Keep moving to the left most node until we reach a leaf
    cost += primary_key_access_cost;
	while(primary2[0].leftchild.n != -1)
    {
        _root = primary2[0].leftchild;
        cost += primary_key_access_cost;
	    readDisk(_root.n, 2);
    }

	// Return the last key of the leaf
    cost += primary_key_access_cost;
	return primary2[0].key;
}

// function to merge two nodes
void merge(coordinate _root)
{
    /*
     * here we merge the nodes with len t - 1
     * all the keys will be copied in the left child
     * and coordinate of this will be stored in the correct 
     * place of parent node
     */
    int len = getlength(1);
    coordinate temp = primary1[_root.x].leftchild;
    cost += primary_key_access_cost;
    readDisk(temp.n, 2);
    int lenleft = getlength(2);
    if(_root.x == len -1)
        readDisk(primary1[_root.x].next.n, 2);
    else
        readDisk(primary1[_root.x + 1].leftchild.n, 2);
    cost += primary_key_access_cost;
    int lenright = getlength(2);
    readDisk(temp.n, 2);
    primary2[lenleft].key = primary1[_root.x].key;
    cost += 2 * primary_key_access_cost;

    coordinate right;
    if(len == 1)
    {
        right = primary1[0].next;
        cost += primary_key_access_cost;
        reloadcell(0, 1);
    }
    else if(_root.x == len -1)
    {
        right = primary1[_root.x].next;
        primary1[_root.x - 1].next = temp;
        cost += 2 * primary_key_access_cost;
        reloadcell(_root.x, 1);
    }
    else
    {
        right = primary1[_root.x + 1].leftchild;
        primary1[_root.x + 1].leftchild = temp;
        cost += 2 * primary_key_access_cost;
        for(unsigned i = _root.x; i < len - 2; i++)
        {
            primary1[i] = primary1[i + 1];
            primary1[i].next.x--;
            cost += 3 * primary_key_access_cost;
        }
        primary1[len - 2] = primary1[len - 1];
        cost += 2 * primary_key_access_cost;
        reloadcell(len - 1, 1);
    }
    writeDisk(_root.n, 1);
    readDisk(right.n, 1);

    // copying all keys to left child and clearing the right child
    for(unsigned i = 0; i < lenright - 1; i++)
    {
        primary2[i + lenleft + 1] = primary1[i];
        primary2[i + lenleft + 1].next.n = temp.n;
        primary2[i + lenleft + 1].next.x = i + lenleft + 1;
        cost += 4 * primary_key_access_cost;
        reloadcell(i, 1);
    }
    primary2[lenright + lenleft] = primary1[lenright - 1];
    cost += 2 * primary_key_access_cost;
    reloadcell(lenright - 1, 1);
    writeDisk(right.n, 1);
    // if the mergeing done at the root level with no of elements are only one
    if(len == 1)
    {
        /*
         * copying back the all elements from the left child to the root
         * and clearing the total left child
         */
        readDisk(_root.n, 1);
        for(unsigned i = 0; i < lenleft + lenright + 1; i++)
        {
            primary1[i] = primary2[i];
            cost += primary_key_access_cost;
            if(i != lenleft + lenright)
            {
                primary1[i].next.n = _root.n;
                cost += primary_key_access_cost;
            }
            reloadcell(i, 2);
        }
        writeDisk(temp.n, 2);
        writeDisk(_root.n, 1);
        temp = _root;
    }
    else
        writeDisk(temp.n, 2);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
        /////* FUNCTION TO DELETE A KEY *////////


void delete(coordinate _root, int _data)
{
    // if root points to nothing return
    if(_root.n == -1)
        return;
    // reading the root node to the primary
    readDisk(_root.n, 1);
    // calculating the root node length
    int len = getlength(1);
    int pos = 0;
    int key = primary1[pos].key;
    cost += primary_key_access_cost;
    // finding the key which is just greater than or equal to data to be deleted
    while (key != -1 && _data > key)
    {
        pos++;
        key = primary1[pos].key;
        cost += primary_key_access_cost;
    }
    // if we found a key to be deleted
    if (key == _data)
    {
        cost += primary_key_access_cost;
        // if the key is in a leaf
        if(primary1[0].leftchild.n == -1)
        {
            /* just reemove the key from the node amd than move all
             * remaing keys  to the left
             */
            for(unsigned i = pos; i < len - 1; i++)
            {
                primary1[i].key = primary1[i + 1].key;
                cost += primary_key_access_cost;
            }
            primary1[len - 2].next = primary1[len - 1].next;
            primary1[len - 1].key = -1;
            cost += 2 * primary_key_access_cost;

            writeDisk(_root.n, 1);
        }
        // not in a leaf
        else
        {
            coordinate temp;
            temp.n = _root.n;
            temp.x = pos;
            readDisk(primary1[pos].leftchild.n, 2);
            cost += primary_key_access_cost;
            int lenleft = getlength(2);
            // if leftchild of key has atleat t keys
            if(lenleft >= t)
            {
                /* swap with minimum element in this left child and then
                 * call this left child recursively for deleting the minimum
                 */
                int pred = getPred(temp);
                primary1[pos].key = pred;
                cost += primary_key_access_cost;
                writeDisk(_root.n, 1);
                delete(primary1[pos].leftchild, pred);
                cost += primary_key_access_cost;
            }
            // if not
            else
            {
                if(pos == len -1)
                    readDisk(primary1[pos].next.n, 2);
                else
                    readDisk(primary1[pos + 1].leftchild.n, 2);
                cost += primary_key_access_cost;
                int lenright = getlength(2);
                // if right child of key has atleat t keys
                if(lenright >= t)
                {
                    /* swap with minimum element in this right child and then
                     * call this right child recursively for deleting the minimum
                     */
                    int succ = getSucc(temp);
                    primary1[pos].key = succ;
                    cost += primary_key_access_cost;
                    writeDisk(_root.n, 1);
                    if(pos == len -1)
                        delete(primary1[pos].next, succ);
                    else
                        delete(primary1[pos + 1].leftchild, succ);
                    cost += primary_key_access_cost;
                }
                // if not
                else
                {
                    // apply merge at cell with the key
                    merge(temp);
                    readDisk(temp.n, 1);
                    if(len == 1)
                        temp = _root;
                    else if(pos == len -1)
                    {
                        temp = primary1[pos - 1].next;
                        cost += primary_key_access_cost;
                    }
                    else
                    {
                        temp = primary1[pos].leftchild;
                        cost += primary_key_access_cost;
                    }
                    // then call deletion recusively on merged node
                    delete(temp, _data);
                }
            }
        }
    }
    // if we found key just greater than the data to be deleted
    else if(_data < key)
    {
        cost += primary_key_access_cost;
        // returning if child node is empty
        if(primary1[pos].leftchild.n == -1)
            return;
        coordinate temp;
        temp.n = _root.n;
        temp.x = pos;
        readDisk(primary1[pos].leftchild.n, 2);
        cost += primary_key_access_cost;
        int lenleft = getlength(2);
        // left child of this key has atleat t keys
        if(lenleft >= t)
        {
            // then recursivel call the left child to delete the data
            delete(primary1[pos].leftchild, _data);
            cost += primary_key_access_cost;
        }
        // if not
        else
        {
            if(pos == len - 1)
                readDisk(primary1[pos].next.n, 2);
            else
                readDisk(primary1[pos + 1].leftchild.n, 2);
            cost += primary_key_access_cost;
            int lenright = getlength(2);
            // if right child of this key has atleat t eys
            if(lenright >= t)
            {
                /*
                 * borrow from this right child to the key's left child and 
                 * then call the left child recursively to delete the data 
                 */
                cell tempcell;
                tempcell.key = primary1[temp.x].key;
                tempcell.next = primary2[0].leftchild;
                primary1[temp.x].key = primary2[0].key;
                cost += 4 * primary_key_access_cost;
                for(unsigned i = 0; i < lenright; i++)
                {
                    primary2[i] = primary2[i + 1];
                    cost += 2 * primary_key_access_cost;
                    if(i < lenright - 2)
                    {
                        primary2[i].next.x--;
                        cost += primary_key_access_cost;
                    }
                }
                if(pos == len - 1)
                    writeDisk(primary1[pos].next.n, 2);
                else
                    writeDisk(primary1[pos + 1].leftchild.n, 2);
                cost += primary_key_access_cost;
                readDisk(primary1[pos].leftchild.n, 2);
                cost += primary_key_access_cost;
                tempcell.leftchild = primary2[lenleft - 1].next;
                primary2[lenleft - 1].next.n = primary1[pos].leftchild.n;
                primary2[lenleft - 1].next.x = lenleft;
                primary2[lenleft] = tempcell;
                cost += 4 * primary_key_access_cost;
                writeDisk(primary1[pos].leftchild.n, 2);
                cost += primary_key_access_cost;
                writeDisk(_root.n, 1);
                delete(primary1[pos].leftchild, _data);
                cost += primary_key_access_cost;
                return;
            }
            // if position of this key first place
            else if(pos == 0)
            {
                /* 
                 * then apply merge at cell with this key and then recursively 
                 * call the left child to delete data
                 */
                merge(temp);
                readDisk(temp.n, 1);
                if(len == 1)
                    temp = _root;
                else if(pos == len -1)
                {
                    temp = primary1[pos - 1].next;
                    cost += primary_key_access_cost;
                }
                else
                {
                    temp = primary1[pos].leftchild;
                    cost += primary_key_access_cost;
                }
                delete(temp, _data);
            }
            /* 
             * if key is not at first level then check for possibility of left child of left key
             */
            else
            {
                readDisk(primary1[pos - 1].leftchild.n, 2);
                cost += primary_key_access_cost;
                int lenleft2 = getlength(2);
                // left child of left key has 
                if(lenleft2 >= t)
                {
                    /*
                     * borrow from this left child to the key's left child and 
                     * then call the left child recursively to delete the data 
                     */
                    temp.x--;
                    pos--;
                    cell tempcell;
                    tempcell.key = primary1[temp.x].key;
                    tempcell.leftchild = primary2[lenleft2 - 1].next;
                    tempcell.next = primary1[temp.x + 1].leftchild;
                    tempcell.next.x = 1;
                    primary1[temp.x].key = primary2[lenleft2 - 1].key;
                    primary2[lenleft2 - 2].next = primary2[lenleft2 - 1].leftchild;
                    cost += 7 * primary_key_access_cost;
                    reloadcell(lenleft2 - 1, 2);
                    writeDisk(primary1[pos].leftchild.n, 2);
                    readDisk(primary1[pos + 1].leftchild.n, 2);
                    cost += 2 * primary_key_access_cost;
                    for(unsigned i = lenright; i > 0; i--)
                    {
                        cost += primary_key_access_cost;
                        primary2[i] = primary2[i - 1];
                        if(i != lenright)
                        {
                            primary2[i].next.x++;
                            cost += primary_key_access_cost;
                        }
                    }
                    primary2[0] = tempcell;
                    cost += primary_key_access_cost;
                    writeDisk(primary1[pos + 1].leftchild.n, 2);
                    cost += primary_key_access_cost;
                    writeDisk(_root.n, 1);
                    // then again recurrsively call the left child of key to delete
                    delete(primary1[pos + 1].leftchild, _data);
                    cost += primary_key_access_cost;
                }
                else
                {
                    /*
                     * if borrowing is not possible and then merge at the cell with key
                     */
                    merge(temp);
                    readDisk(temp.n, 1);
                    if(len == 1)
                    temp = _root;
                    else if(pos == len -1)
                    {
                        temp = primary1[pos - 1].next;
                        cost += primary_key_access_cost;
                    }
                    else
                    {
                        temp = primary1[pos].leftchild;
                    }
                    // then call recursively
                    delete(temp, _data);
                }
            }
        }
    }
    // if there are no keys greater than the data to be deleted in the node
    else
    {
        pos--;
        cost += primary_key_access_cost;
        // returning if child is empty
        if(primary1[pos].next.n == -1)
            return;
        coordinate temp;
        temp.n = _root.n;
        temp.x = pos;
        readDisk(primary1[pos].next.n, 2);
        cost += primary_key_access_cost;
        int lenright = getlength(2);
        // if the right most child has atleast t keys
        if(lenright >= t)
        {
            // calling recursively the right most child to delete
            delete(primary1[pos].next, _data);
            cost += primary_key_access_cost;
        }
        // if not
        else
        {
            readDisk(primary1[pos].leftchild.n, 2);
            cost += primary_key_access_cost;
            int lenleft = getlength(2);
            // if left child of right most key has atleast t keys
            if(lenleft >= t)
            {
                /*
                 * borrowing from this node to the right most node
                 */
                cell tempcell;
                tempcell.key = primary1[temp.x].key;
                tempcell.leftchild = primary2[lenleft - 1].next;
                tempcell.next = primary1[temp.x + 1].leftchild;
                tempcell.next.x = 1;
                primary1[temp.x].key = primary2[lenleft - 1].key;
                primary2[lenleft - 2].next = primary2[lenleft - 1].leftchild;
                reloadcell(lenleft - 1, 2);
                writeDisk(primary1[pos].leftchild.n, 2);
                readDisk(primary1[pos].next.n, 2);
                cost += 9 * primary_key_access_cost;
                for(unsigned i = lenright; i > 0; i--)
                {
                    primary2[i] = primary2[i - 1];
                    cost += primary_key_access_cost;
                    if(i != lenright)
                    {
                        primary2[i].next.x++;
                        cost += primary_key_access_cost;
                    }
                }
                primary2[0] = tempcell;
                cost += primary_key_access_cost;
                writeDisk(primary1[pos].next.n, 2);
                cost += primary_key_access_cost;
                writeDisk(_root.n, 1);
                // and then calling the rightmost child for deletion
                delete(primary1[pos].next, _data);
                cost += primary_key_access_cost;
            }
            // if left and right child doent have atleast t keys
            else
            {
                // we call the merge function at the right most key
                merge(temp);
                readDisk(temp.n, 1);
                if(len == 1)
                    temp = _root;
                else if(pos == len -1)
                {
                    temp = primary1[pos - 1].next;
                    cost += primary_key_access_cost;
                }
                else
                {
                    temp = primary1[pos].leftchild;
                    cost += primary_key_access_cost;
                }
                // then calling recursively the merged node
                delete(temp, _data);
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////