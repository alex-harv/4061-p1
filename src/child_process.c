#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include "hash.h"

#define PATH_MAX 1024

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: ./child_process <blocks_folder> <hashes_folder> <N> <child_id>\n");
        return 1;
    }

    // TODO: If the current process is a leaf process, read in the associated block file 
    // and compute the hash of the block.

    // TODO: If the current process is not a leaf process, spawn two child processes using  
    // exec() and ./child_process. 

    // TODO: Wait for the two child processes to finish

    // TODO: Retrieve the two hashes from the two child processes from output/hashes/
    // and compute and output the hash of the concatenation of the two hashes.
    
    // Command-line arguments into local variable
    char *blocks_folder = argv[1];
    char *hashes_folder = argv[2];
    int input_n = atoi(argv[3]);
    int input_child_id = atoi(argv[4]);
    char block_filename[PATH_MAX];
    char hash_filename[PATH_MAX];
    char result_hash[SHA256_BLOCK_SIZE * 2 + 1];



    pid_t pid = fork();
    if (pid == 0) {
        //left tree 
        printf("left tree input_n=%d",input_n);
        if (input_n == 1) {
            //this is a leaf node
            snprintf(block_filename, sizeof(block_filename), "%s/%d.txt", blocks_folder, input_child_id);
            hash_data_block(result_hash, block_filename);

            snprintf(hash_filename, sizeof(hash_filename), "%s/%d.txt", hashes_folder, input_child_id);
            FILE *f = fopen(hash_filename, "w");
            if (!f) {
                perror("Error: Opening hash file");
                exit(1);
            }
            fprintf(f, "%s", result_hash);
            printf("left tree leaf node hash written\n");
            fclose(f);
            exit(0);
        }
        //this is not a leaf node
        
        int left_child_id = 2 * input_child_id + 1;
        char n[10];
        char child_id[10];
        snprintf(n, sizeof(n), "%d", input_n / 2);
        snprintf(child_id, sizeof(child_id), "%d", left_child_id);
        printf("left tree recursion n= %s child_id= %s\n",n,child_id);

        char *argv[] = {"./child_process", blocks_folder, hashes_folder, n, child_id, NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed at child_id:%s, and N:%s \n", child_id, n);
            exit(1);
        }
        exit(0);
    } 




    else {
        //right tree
        printf("right tree input_n=%d",input_n);
        if (input_n == 1) {
            //this is a leaf node
            snprintf(block_filename, sizeof(block_filename), "%s/%d.txt", blocks_folder, input_child_id);
            hash_data_block(result_hash, block_filename);

            snprintf(hash_filename, sizeof(hash_filename), "%s/%d.txt", hashes_folder, input_child_id);
            FILE *f = fopen(hash_filename, "w");
            if (!f) {
                perror("Error: Opening hash file");
                exit(1);
            }
            
            fprintf(f, "%s", result_hash);
            printf("right tree leaf node hash written\n");

            fclose(f);
            
            exit(0);
           
        }
        //this is not a leaf node
        
        

        int right_child_id = 2 * input_child_id + 2;
        char n[10];
        char child_id[10];
        snprintf(n, sizeof(n), "%d", input_n / 2);
        snprintf(child_id, sizeof(child_id), "%d", right_child_id);

        printf("right tree recursion n= %s child_id= %s\n",n,child_id);
        char *argv[] = {"./child_process", blocks_folder, hashes_folder, n, child_id, NULL};
        if (execvp(*argv, argv) < 0) {
            printf("ERROR: exec failed at child_id:%s, and N:%s \n", child_id, n);
            exit(1);
        }

        char left_hash[SHA256_BLOCK_SIZE * 2 + 1];
        char right_hash[SHA256_BLOCK_SIZE * 2 + 1];

        snprintf(hash_filename, sizeof(hash_filename), "%s/%d.txt", hashes_folder, 2 * input_child_id + 1);
        FILE *left_f = fopen(hash_filename, "r");
        if (!left_f) {
            perror("Error: Opening left hash");
            exit(1);
        }
        fscanf(left_f, "%s", left_hash);
        fclose(left_f);

        snprintf(hash_filename, sizeof(hash_filename), "%s/%d.txt", hashes_folder, 2 * input_child_id + 2);
        FILE *right_f = fopen(hash_filename, "r");
        if (!right_f) {
            perror("Error: Opening right hash");
            exit(1);
        }
        fscanf(right_f, "%s", right_hash);
        fclose(right_f);


        compute_dual_hash(result_hash, left_hash, right_hash);
        printf("computed dual hash in right tree\n");

        snprintf(hash_filename, sizeof(hash_filename), "%s/%d.txt", hashes_folder, input_child_id);
        FILE *f = fopen(hash_filename, "w");
        if (!f) {
            perror("Error: Opening hash file");
            exit(1);
        }
        fprintf(f, "%s", result_hash);
        fclose(f);


        wait(NULL);
        exit(0);
        printf("right tree parent exited\n");
    }
}
