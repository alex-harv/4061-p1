#include "utils.h"
#include "print_tree.h"

// ##### DO NOT MODIFY THESE VARIABLES #####
char *blocks_folder = "output/blocks";
char *hashes_folder = "output/hashes";
char *visualization_file = "output/visualization.txt";

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        // N is the number of data blocks to split <file> into (should be a power of 2)
        // N will also be the number of leaf nodes in the merkle tree
        printf("Usage: ./merkle <file> <N>\n");
        return 1;
    }

    // TODO: validate command line arguments
    char *input_file = argv[1];
    int n = atoi(argv[2]);

    // ##### DO NOT REMOVE #####
    setup_output_directory(blocks_folder, hashes_folder);
    partition_file_data(input_file, n, blocks_folder);

    // Start the recursive merkle tree computation by spawning first child process (root)
    // ./child_process <blocks_folder> <hashes_folder> <N> <child_id>
    char *argv[] = {"./child_process", blocks_folder, hashes_folder, n, 0, NULL};
    execvp(*argv, argv);





    // ##### DO NOT REMOVE #####
    #ifndef TEST_INTERMEDIATE
    // Visually display the merkle tree using the output in the hashes_folder
        print_merkle_tree(visualization_file, hashes_folder, n);
    #endif

    return 0;
}