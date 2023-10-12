#include "utils.h"

// Create N files and distribute the data from the input file evenly among them
// See section 3.1 of the project writeup for important implementation details
void partition_file_data(char *input_file, int n, char *blocks_folder)
{
    // Hint: Use fseek() and ftell() to determine the size of the file

    FILE *fp = fopen(input_file, "rb");
    if (!fp)
    {
        perror("Error: Opening input file");
    }

    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    int chunkSize = fileSize / n;
    int restOfData = fileSize % n;

    size_t byteRead;

    for (int i = 0; i < n; i++)
    {
        if (i == n - 1)
        {
            chunkSize += restOfData;
        }
        char buffer[chunkSize];

        byteRead = fread(buffer, 1, chunkSize, fp);
        if (byteRead <= 0)
        {
            printf("Error: reading or eof error\n");
            exit(1);
        }

        char fileName[256];
        snprintf(fileName, sizeof(fileName), "%s/%d.txt", blocks_folder, i);

        FILE *chunk = fopen(fileName, "w");
        if (!chunk)
        {
            printf("Error: Creating chunk file %s\n", fileName);
            exit(1);
        }

        fwrite(buffer, 1, byteRead, chunk);
        fclose(chunk);
    }

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