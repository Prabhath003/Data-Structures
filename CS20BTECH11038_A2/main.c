#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Including the implementation of binary tree and Avl tree */
#include "binarytree.h"
#include "avlTree.h"
/* Including the implementation of stack and queue */
#include "stack.h"
#include "queue.h"

/* functions to read the file, create library */
void readFile(binaryTree* _tree, char* fileName);
/* function to delete from library */
void deleteFromLibrary(binaryTree* _tree);
/* function to create Playlist */
void createPlaylist(binaryTree* _library, avlTree* _tree);
/* function to delete from Playlist */
void deleteFromPlaylist(avlTree* _tree);
/* funtion to play the songs from Playlist*/
void playPlaylist(treeNode* _PlaylistRoot, stack* _recents);

int main()
{
    // creating the libary using binary search tree
    binaryTree *giftTree = initBinaryTree();
    // calling function to read the file and create library
    readFile(giftTree, "gift");

    // printing the inorder traversal of library
    printf("\"The songs' library is formed with following songs\"\n");
    printInorderTraverse(giftTree->root);
    printf("\n");

    // calling function to delete from library
    deleteFromLibrary(giftTree);

    // the final library
    printf("\"The final songs' library\"\n");
    printInorderTraverse(giftTree->root);
    printf("\n");

    // creating the Playlist using AVL tree
    avlTree* Playlist = initAvlTree();
    // calling function to to insert into Playist
    createPlaylist(giftTree, Playlist);

    // printing the inorder traversal of Playlist
    printf("\"The created Playlist is\"\n");
    printInorderTraverse(Playlist->root);
    printf("\n");

    // calling functiont to delete songs from the playlist
    deleteFromPlaylist(Playlist);

    // printing the final playlist
    printf("\"The final Playlist\"\n");
    printInorderTraverse(Playlist->root);
    printf("\n");

    // creating stack to play the previous songs
    stack* recents = initStack();
    // calling function to play the playlist
    playPlaylist(Playlist->root, recents);

    printf("\"Playlist completed!\"\n");
    printf("\"Bye!\"\n");

    return EXIT_SUCCESS;
}


void readFile(binaryTree* _tree, char* fileName)
{
    // creating the FILE variable
    FILE *giftFile;
    char ch; // variable to read the file
    char song[data_length]; // variable to store the song name temporarly
    song[0] = '\0';
    giftFile = fopen(fileName, "r"); // opening the file
    while(1)
    {
        // reading the variable character by character
        ch = fgetc(giftFile);
        if(ch == EOF)
        {
            // inserting the song reading its full name
            insertBinaryTreeNode(_tree, song);
            break;
        }
        else if(ch == ':')
        {
            // checking the ending of song name
            insertBinaryTreeNode(_tree, song);
            song[0] = '\0';
        }
        else
        {
            // making the song name same after reading character by character
            unsigned i = 0;
            while(song[i] != '\0')
                i++;
            song[i] = ch;
            song[i+1] = '\0';
        }
    }
}

void deleteFromLibrary(binaryTree* _tree)
{
    // creating a variable to store the input
    char song[data_length];
    song[0] = '\0';
    printf("\"Enter the songs that you want to delete from the library, end with -1\"\n");
    while(true)
    {
        scanf("%s", song);
        // ending the delete function
        if(strcmp(song, "-1") == 0)
            break;
        // deleting the Node if it is available
        else if(findTreeNode(_tree->root, song) != NULL)
        {
            deleteBinaryTreeData(_tree, song);
            printf("\"found and deleted\"\n");
        }
        else // saying its nota available to delete
            printf("\"Not found in the library to delete\"\n");
    }
}

void createPlaylist(binaryTree* library, avlTree* _tree)
{
    // creating a variable store the input
    char song[data_length];
    song[0] = '\0';
    printf("\"Create playlist, end with -1\"\n");
    while(true)
    {
        scanf("%s", song);
        // checking condition to end the function
        if(strcmp(song, "-1") == 0)
            break;
        // inserting into playlist if available in library
        else if(findTreeNode(library->root, song) != NULL)
        {
            insertAvlTree(_tree, song);
            printf("\"Found in library and inserted\"\n");
        }
        else // saying not available in library
            printf("\"Not found in library\"\n");
    }
}

void deleteFromPlaylist(avlTree* _tree)
{
    // creating a variable to store the input
    char song[data_length];
    song[0] = '\0';
    printf("\"If you want to delete songs from playlist\"\n");
    printf("\"Enter the songs that you want to delete, end with -1\"\n");
    while(true)
    {
        scanf("%s", song);
        // checking the condition to end the function
        if(strcmp(song, "-1") == 0)
            break;
        // checking whether available in the playlist before deleting
        else if(findTreeNode(_tree->root, song) != NULL)
        {
            deleteAvlTreeData(_tree, song);
            printf("\"found and deleted\"\n");
        }
        else // saying not available to delete
            printf("\"Not found in the Playlist to delete\"\n");
    }
}

void playPlaylist(treeNode* _PlaylistRoot, stack *_recents)
{
    // checking if not empty node
    if(_PlaylistRoot == NULL)
        return;
    // call the left node recursively
    playPlaylist(_PlaylistRoot->leftChild, _recents);
    // creating a temporary variable to store the input
    char *navigation = (char*)malloc(data_length * sizeof(char));
    while(true)
    {
        printf("\"Play next song, previous songs or end? n/p:k/e\"\n");
        scanf("%s", navigation); // taking the input
        if(strcmp(navigation, "n") == 0)
        {
            // playing the song and pushing into recents stack
            printf("\"Playing %s\"\n", _PlaylistRoot->data);
            push(_recents, _PlaylistRoot->data);
            break;
        }
        // ending the programme
        else if(strcmp(navigation, "e") == 0)
        {
            printf("\"Bye!\"\n");
            exit(0);
        }
        // playing the previous songs
        else if(navigation[0] == 'p' && strlen(navigation) > 2) // if p is present then it will play from the recents
        {
            // temperory queue to store the songs played from recents
            queue* temp_recents = initQueue();
            // it checks for the number of previos song to played
            char *part_with_num = navigation + 2;
            for(unsigned i = 0; i < atoi(part_with_num); i++)
            {
                if(!isEmpty(*_recents))
                {
                    printf("Playing %s\n", top(*_recents));
                    enQueue(temp_recents, pop(_recents));
                }
                else // if recents have no songs to play
                {
                    printf("\"There aren’t %d previous songs\"\n", atoi(part_with_num));
                    break;
                }
            }

            // again copying back from the temporary queue to the recents
            unsigned j = getSize(*temp_recents);
            for(unsigned i = 0; i < j; i++)
                push(_recents, deQueue(temp_recents));

            free(temp_recents);
        }
    }
    free(navigation);

    // calling the right node recursively
    playPlaylist(_PlaylistRoot->RightChild, _recents);
}