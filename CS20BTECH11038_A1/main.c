#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* including the impletation of lists, stacks, queues */
#include "list.h"
#include "stack.h"
#include "queue.h"

/* functions to create library, playlist and to play songs */ 
void createLibrary(list* _library);
void createPlaylist(list *_library, queue* _playlist);
void playPlaylist(queue* _playlist, stack* _recents);

int main()
{
    // creating the library using list
    list* songs_library = initList();
    // creating the playlist using queue
    queue* songs_playlist = initQueue();
    // creating the stack using stack
    stack* recents = initStack();

    // calling functions to create libray, playlist of songs and to play them
    createLibrary(songs_library);
    createPlaylist(songs_library, songs_playlist);
    playPlaylist(songs_playlist, recents);

    printf("\"Bye\"\n");

    // making the pointers used free
    free(songs_library);
    free(songs_playlist);
    free(recents);
    
    return EXIT_SUCCESS;
}

void createLibrary(list* _library)
{
    printf("\"Enter all songs (as strings), End with \"-1\"\"\n");

    // creating a temporary variable to store the input
    char *song = (char*)malloc(data_length * sizeof(char));

    /* while loop to take input continuosly until -1 is entered */
    while(true)
    {
        scanf("%s", song);
        if(strcmp(song, "-1") == 0) break;
        else if(find(*_library, song) == -1)
            pushFront(_library, song); // adding the entered song name into the library
    }
    free(song);
    printf("\"All songs entered!\"\n");
}

void createPlaylist(list *_library, queue* _playlist)
{
    // if the library is empty we can't add songs to the playlist
    if(isEmpty(*_library))
    {
        printf("\"There are no songs in library to add to playlist\n\"");
        return;
    }

    printf("\"Create Playlist, end with \"-1\"\"\n");

    // creating a temporary variable to store the input
    char *song = (char*)malloc(data_length * sizeof(char));
    while(true)
    {
        scanf("%s", song);
        if(strcmp(song, "-1") == 0) break;
        else
        {
            if(find(*_library, song) != -1)
            {
                enQueue(_playlist, song); // adding the song to playlist queue
                printf("\"Found, Enqueued\"\n");
            }
            else
                printf("\"Not Found in the song library\"\n");
        }
    }

    free(song);
    printf("\"Playlist Ready!\"\n");
}

void playPlaylist(queue* _playlist, stack* _recents)
{
    // creating a temporary variable to store the input
    char *song = (char*)malloc(data_length * sizeof(char));
    // while loop to take the input continuously until "e" is enterd
    while(true)
    {
        // if the playlist is empty we can't play any songs
        if(isEmpty(*_playlist))
        {
            printf("The playlist is empty\n");
            printf("You can only play from recents(p:k) or end(e)\n");
        }
        else // we can play the songs  from the recents stack;
            printf("\"Play next song, previous songs or end? n/p:k/e\"\n");

        scanf("%s", song);

        if(strcmp(song, "e") == 0) break; // if "e" entered the function will terminate you
        else if(strcmp(song, "n") == 0 && !isEmpty(*_playlist))
        {
            // getting the data from the queue of playlists and pushing it into recernts stack
            push(_recents, deQueue(_playlist));
            printf("Playing %s\n", top(*_recents));
        }
        else if(song[0] == 'p' && strlen(song) > 2) // if p is present then it will play from the recents
        {
            // temperory queue to store the songs played from recents
            queue* temp_recents = initQueue();
            // it checks for the number of previos song to played
            char *part_with_num = song + 2;
            printf("%d\n", atoi(part_with_num));
            for(unsigned i = 0; i < atoi(part_with_num); i++)
            {
                if(!isEmpty(*_recents))
                {
                    printf("\"Playing %s\"\n", top(*_recents));
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

    free(song);
}