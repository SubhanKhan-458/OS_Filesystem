#include "../include/globals.h"

void dump_to_file(const char *, const char *, char *, int);
int init_dev(const char *, int *);
void close_dev(int *);

int main () {
    int fd;

    printf("TOTAL_NO_OF_BLOCKS = %d\n", TOTAL_NO_OF_BLOCKS);
    printf("TOTAL_NO_OF_INODE_BLOCKS = %d\n", TOTAL_NO_OF_INODE_BLOCKS);
    printf("TOTAL_NO_OF_INDIRECT_NODE_BLOCKS = %d\n", TOTAL_NO_OF_INDIRECT_NODE_BLOCKS(sizeof(inode), sizeof(indirect_node)));
    printf("TOTAL_NO_OF_DENTRY_BLOCKS = %d\n", TOTAL_NO_OF_DENTRY_BLOCKS(sizeof(inode), sizeof(dentry)));
    printf("TOTAL_NO_OF_INODES = %d\n", TOTAL_NO_OF_INODES(sizeof(inode)));
    printf("TOTAL_NO_OF_INDIRECT_NODES = %d\n", TOTAL_NO_OF_INDIRECT_NODES(sizeof(inode)));
    printf("NO_OF_INODES_PER_BLOCK = %d\n", NO_OF_INODES_PER_BLOCK(sizeof(inode)));
    printf("NO_OF_INDIRECT_NODES_PER_BLOCK = %d\n", NO_OF_INDIRECT_NODES_PER_BLOCK(sizeof(indirect_node)));
    printf("NO_OF_DENTRY_PER_BLOCK = %d\n", NO_OF_DENTRY_PER_BLOCK(sizeof(dentry)));
    printf("SUPER_BLOCK_INDEX_NO = %d\n", SUPER_BLOCK_INDEX_NO);
    printf("DENTRY_BLOCKS_INDEX_NO = %d\n", DENTRY_BLOCKS_INDEX_NO);
    printf("INODE_BLOCKS_INDEX_NO = %d\n", INODE_BLOCKS_INDEX_NO(sizeof(inode), sizeof(dentry)));
    printf("INDIRECT_NODE_BLOCKS_INDEX_NO = %d\n", INDIRECT_NODE_BLOCKS_INDEX_NO(sizeof(inode), sizeof(dentry)));


    // init_dev("/home/shaheer/OS_Filesystem/temp/foo.img", &fd);

    // temp
    // char * bl = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    // read_block(&fd, (void *) bl, 0);
    // dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "w", bl, BLOCK_SIZE);
    // free(bl);

    // close_dev(&fd);

    return 0;
}

/**
 * @brief Dumps the contents of a buffer to a specified file
 * 
 * It will stop writing to the file when the target size is reached
 * or when a null-terminator ('\0') is found
 * 
 * @param path Path to our dump file
 * @param modes File modes ("r", "w", "a"...)
 * @param data The data we want to dump
 * @param n Size of data (in bytes)
 */
void dump_to_file(const char * path, const char * modes, char * data, int n) {
    FILE * fp = fopen(path, modes);
    int i;

    if (fp == NULL) {
        pprintf("Unable to open file for dumping... [dump_to_file]");
        return;
    }

    for (i = 0; (i < n) && (data[i] != '\0'); i++) {
        fprintf(fp, "%c ", data[i]);
    }

    fclose(fp);

    return;
}

/**
 * @brief Initializes the block device, from creating the device file to 
 * populating the inodes and writing the superblock
 * 
 * Compares the first 4 bytes of the file with the filesystem's magic number, 
 * if it is the same, it only initializes the bitmaps, else it initializes everything
 * 
 * @param path Path to block device (file)
 * @param fd File descriptor (we write the returned fd to this)
 * @return (int) -1 for error, 1 for ok
 */
int init_dev(const char * path, int * fd) {
    if (path == NULL || fd == NULL) {
        pprintf("Invalid parameters provided [init_dev]");
        return -1;
    }

    *fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (*fd == -1) {
        pprintf("Unable to open file [init_dev]");
        perror("open");
        return -1;
    }

    // read first block
    // if first block has superblock which contains correct magic number
    // then no need to initialize inodes and everything, just the bitmaps
    // else initialize everything from scratch
    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [init_dev]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, 0) == -1) {
        pprintf("Unable to read_block [init_dev]");
        free(block_buffer);
        return -1;
    }

    super_block sb;

    memcpy(&sb.magic_number, block_buffer, sizeof(__uint32_t));
    
    if (sb.magic_number != FS_MAGIC_NUMBER) {
        // initialize inode blocks, data blocks etc.
        // everything gets initialized from scratch
        // first time

        // clean all the blocks first
        if (clean_all_blocks(fd) == -1) {
            pprintf("Unable to clean all blocks [init_dev]");
            free(block_buffer);
            return -1;
        }

        sb.magic_number = (u_int32_t) FS_MAGIC_NUMBER;
        sb.total_blocks = (u_int32_t) TOTAL_NO_OF_BLOCKS;
        sb.total_inode_blocks = (u_int32_t) TOTAL_NO_OF_INODE_BLOCKS;
        sb.total_no_of_inodes = (u_int32_t) TOTAL_NO_OF_INODES(sizeof(inode));

        memcpy(block_buffer, &sb, sizeof(super_block));

        // write super block
        if (write_block(fd, (void *) block_buffer, SUPER_BLOCK_INDEX_NO) == -1) {
            pprintf("Unable to write super block [init_dev]");
            free(block_buffer);
            return -1;
        }
    }

    // initialize bitmaps here

    free(block_buffer);

    return 1;
}

/**
 * @brief Closes the opened block device file
 * 
 * @param fd File descriptor
 */
void close_dev(int * fd) {
    if (fd == NULL || *fd == -1) {
        pprintf("Invalid parameters provided [init_dev]");
        return;
    }

    close(*fd);

    return;
}
