#include "../include/globals.h"

#include "../include/globals.h"

/**
 * @brief Initializes the dentry blocks in the block device
 * 
 * @param fd File descriptor
 * @return (int) -1 for error, 1 for ok 
 */
int initialize_dentry_blocks(int * fd) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [initialize_dentry_blocks]");
        return -1;
    }

    dentry temp;
    temp.inode_index = 0; // 0 indicates empty, subtract 1 from pointer to get the actual pointer

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [initialize_dentry_blocks]");
        perror("malloc");
        return -1;
    }

    int i;

    // ensures that any remaining bytes in a block are always null-terminators
    memset(block_buffer, '\0', BLOCK_SIZE);

    // create a block filled with empty dentry
    for (i = 0; i < NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY); i++) {
        memcpy(block_buffer + (i * SIZEOF_DENTRY), &temp, SIZEOF_DENTRY);
    }

    // write dentry blocks starting from the first dentry block index till inode blocks index
    // (it is the same as total no of dentry blocks)
    for (i = DENTRY_BLOCKS_INDEX_NO; i < INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i++) {
        if (write_block(fd, (void *) block_buffer, i) == -1) {
            pprintf("Unable to write block [initialize_dentry_blocks]");
            free(block_buffer);
            return -1;
        }
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Saves directory entry to block device
 * 
 * @param fd File descriptor
 * @param buffer Directory entry data to write
 * @param dentry_index The target index to write at
 * @return (int) -1 for error, 1 for ok
 */
// try to place dentry early on, i.e. from index 0...
int write_dentry(int * fd, dentry * buffer, int dentry_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        pprintf("Invalid parameters provided [write_dentry]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_DENTRY - 1) is equal to the TOTAL_NO_OF_DENTRY,
    // hence when >= is used, not just >
    if (dentry_index < 0 || dentry_index >= TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY)) {
        pprintf("Invalid dentry index provided [write_dentry]");
        return -1;
    }

    int block_no = (int) (dentry_index / NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY));
    int dentry_blocks_index = DENTRY_BLOCKS_INDEX_NO;
    if ((block_no + dentry_blocks_index) < 0 || (block_no + dentry_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block no calculated [write_dentry]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [write_dentry]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, dentry_blocks_index + block_no) == -1) {
        pprintf("Unable to read block [write_dentry]");
        free(block_buffer);
        return -1;
    }

    int offset = (dentry_index - (block_no * NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY))) * SIZEOF_DENTRY;
    memcpy(block_buffer + offset, buffer, SIZEOF_DENTRY);

    if (write_block(fd, (void *) block_buffer, dentry_blocks_index + block_no) == -1) {
        pprintf("Unable to write block [write_dentry]");
        free(block_buffer);
        return -1;
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Saves the targetted dentry's data to the dentry buffer
 * 
 * @param fd File descriptor
 * @param buffer The dentry buffer, where we would save the data
 * @param dentry_index The target dentry's index
 * @return (int) -1 for error, 1 for ok 
 */
int read_dentry(int * fd, dentry * buffer, int dentry_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        pprintf("Invalid parameters provided [read_dentry]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INODES - 1) is equal to the TOTAL_NO_OF_INODES,
    // hence when >= is used, not just >
    if (dentry_index < 0 || dentry_index >= TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY)) {
        pprintf("Invalid inode index provided [read_dentry]");
        return -1;
    }

    int block_no = (int) (dentry_index / NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY));
    int dentry_blocks_index = DENTRY_BLOCKS_INDEX_NO;
    if ((block_no + dentry_blocks_index) < 0 || (block_no + dentry_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block no calculated [read_dentry]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [read_dentry]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, dentry_blocks_index + block_no) == -1) {
        pprintf("Unable to read block [read_dentry]");
        free(block_buffer);
        return -1;
    }

    int offset = (dentry_index - (block_no * NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY))) * SIZEOF_DENTRY;
    memcpy(buffer, block_buffer + offset, SIZEOF_DENTRY);

    free(block_buffer);
    return 1;
}

/**
 * @brief Cleans a targetted dentry, resetting all attributes to the "empty state"
 * 
 * "empty state" is the initial state, where the inode_index = 0
 * 
 * @param fd File descriptor
 * @param inode_index The targetted dentry's index
 * @return (int) -1 for error, 1 for ok 
 */
int clean_dentry(int * fd, int dentry_index) {
    dentry temp = {
        .inode_index = 0, // 0 indicates empty, subtract 1 from pointer to get the actual pointer
    };

    if (write_dentry(fd, &temp, dentry_index) == -1) {
        pprintf("Unable to clean dentry [clean_dentry]");
        return -1;
    }

    return 1;
}

/**
 * @brief Dumps a dump_dentry's data to stdout
 * 
 * @param buffer The dump_dentry whose data we want to dump
 */
void dump_dentry(dentry * buffer) {
    if (buffer == NULL) {
        printf("Invalid parameters provided [dump_dentry]");
        return;
    }

    printf("--------------------------------------------\n");
    printf("DENTRY\n");
    printf("--------------------------------------------\n");
    printf("filename = %s,\n", buffer->filename);
    printf("inode index = %d\n", buffer->inode_index);

    printf("--------------------------------------------\n");

    return;
}
