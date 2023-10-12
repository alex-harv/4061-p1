#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"

#define PATH_MAX 1024

void recurse(char *blocks_folder, char *hashes_folder, int n, int child_id) {
    //this is to clean up code

}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id>\n");
        return 1;
    }

    char *blocks_folder = argv[1];
    char *hashes_folder = argv[2];
    int n = atoi(argv[3]);
    int child_id = atoi(argv[4]);

    if(n == 1) {
        exit(0);
    }




    pid_t pid = fork();
    if (pid == 0)
    {
        //TODO: do left of tree

        // ./child_process <blocks_folder> <hashes_folder> <N> <child_id>
        char *argv[] = {"./child_process", blocks_folder, hashes_folder, n/2, child_id*2+1, NULL};
        if (execvp(*argv, argv) < 0)
        {
            printf("ERROR: exec failed at child_id:%d, and N:%d \n", child_id,n);
            exit(1);
        }
        exit(0);
    }
    else
    {
        //TODO: do right of tree
        char *argv[] = {"./child_process", blocks_folder, hashes_folder, n/2, child_id*2+2, NULL};
        if (execvp(*argv, argv) < 0)
        {
            printf("ERROR: exec failed at child_id:%d, and N:%d \n", child_id,n);
            exit(1);
        }
        


        //wait for left to finish
        wait(NULL);
        exit(0);
    }

    
}

