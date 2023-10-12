#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"

#define PATH_MAX 1024


int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id>\n");
        return 1;
    }

    char *blocks_folder = argv[1];
    char *hashes_folder = argv[2];
    int input_n = atoi(argv[3]);
    char* n;
    int input_child_id = atoi(argv[4]);
    char* child_id;

    if (input_n == 1)
    {
        exit(0);
    }

    //converting to pass to command line arguments
    
    sprintf(n, "%d", input_n/2); //n is always divided by 2

    

    

    /*
        left=child (pid==0)
        right=parent (pid>0)
        left: child_id = 2*parent_id + 1
        right: child_id = 2*parent_id + 2
        right: waits for left to finish then exits
        both: n = input_n/2
    */
    pid_t pid = fork();
    if (pid == 0)
    {
        // TODO: do left of tree
        
        //converting to pass to command line arguments
        
        sprintf(child_id, "%d", input_child_id*2+1);

        // ./child_process <blocks_folder> <hashes_folder> <N> <child_id>
        char *argv[] = {"./child_process", blocks_folder, hashes_folder, n, child_id, NULL};
        if (execvp(*argv, argv) < 0)
        {
            printf("ERROR: exec failed at child_id:%d, and N:%d \n", child_id, n);
            exit(1);
        }
        exit(0);
    }
    else
    {
        // TODO: do right of tree

        
        sprintf(child_id, "%d", input_child_id*2+2);

        // ./child_process <blocks_folder> <hashes_folder> <N> <child_id>
        char *argv[] = {"./child_process", blocks_folder, hashes_folder, n, child_id, NULL};
        if (execvp(*argv, argv) < 0)
        {
            printf("ERROR: exec failed at child_id:%d, and N:%d \n", child_id, n);
            exit(1);
        }

        // wait for left to finish
        wait(NULL);
        exit(0);
    }
}
