#include "utils.h"
#include "string.h"

// Use PATH_MAX when creating char arrays for file names

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder)
{
    // Hint: Use fseek() and ftell() to determine the size of the file
    

    // create N files
    char* filename;
    for (int x = 0; x < n; x++)
    {
        if (x<10){
            filename=*blocks_folder+"/hash/00"+x;
        }
        else if (x<100){
            filename=*blocks_folder+"/hash/0"+x;
        }
        else{
            filename=*blocks_folder+"/hash/"+x;
        }
        

        char *argv[] = {"touch", filename, NULL}; // create file
        if (execvp(*argv, argv) < 0)
        {
            printf("ERROR: file creation failed\n");
            exit(1);
        }
    }

    FILE *fp = fopen(input_file, "r");
    if (fp == NULL)
    {
        printf("ERROR: file not foundd\n");
        exit(1);
    }

    // get size of file
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    // reset fp
    fseek(fp, 0L, SEEK_SET);

    // get block size
    long blockSize = size / n;
    int rem = size % n;

    // distribute data
    // use fseek, fread, fwrite


    fclose(fp);
}

// ##### DO NOT MODIFY THIS FUNCTION #####
void setup_output_directory(char *block_folder, char *hash_folder)
{
    // Remove output directory
    pid_t pid = fork();
    if (pid == 0)
    {
        char *argv[] = {"rm", "-rf", "./output/", NULL};
        if (execvp(*argv, argv) < 0)
        {
            printf("ERROR: exec failed\n");
            exit(1);
        }
        exit(0);
    }
    else
    {
        wait(NULL);
    }

    sleep(1);

    // Creating output directory
    if (mkdir("output", 0777) < 0)
    {
        printf("ERROR: mkdir output failed\n");
        exit(1);
    }

    sleep(1);

    // Creating blocks directory
    if (mkdir(block_folder, 0777) < 0)
    {
        printf("ERROR: mkdir output/blocks failed\n");
        exit(1);
    }

    // Creating hashes directory
    if (mkdir(hash_folder, 0777) < 0)
    {
        printf("ERROR: mkdir output/hashes failed\n");
        exit(1);
    }
}
