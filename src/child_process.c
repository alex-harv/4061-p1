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


    /*
    ./child_process blocks_folder hashes_folder n child_id
    where n=leafs and child_id = id of the child
    */


    
    

    /*
        left=child (pid==0)
        right=parent (pid>0)
        left: child_id = 2*parent_id + 1
        right: child_id = 2*parent_id + 2
        right: waits for left to finish then exits
        both: n = input_n/2
    */
    //converting to pass to command line arguments
    

    pid_t pid = fork();
    if (pid == 0)
    {
        //this branch is the left child and exits before the right child process

        if (input_n == 1){
            //this is a leaf node process
            //TODO: hash the file
            // how do we hash the file corresponding to 
            //filename =child_id

            exit(0);
        }
        
        //this is a non-leaf node process
        //TODO: hash the two children
        
        
        
        //converting to pass to command line arguments
        sprintf(child_id, "%d", input_child_id*2+1);
        sprintf(n, "%d", input_n/2); 

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
        //this branch is the right child and waits for left to finish then exits
        if (input_n == 1){
            //this is a leaf node process
            //TODO: hash the file

            exit(0);
        }
        
        //this is a non-leaf node process
        //TODO: hash the two children

        
        sprintf(child_id, "%d", input_child_id*2+2);
        sprintf(n, "%d", input_n/2); 

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
