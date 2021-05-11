#include "../include/globals.h"

/**
 * @brief Saves the specified block's content into the provided buffer from the target file
 * 
 * @param fd File descriptor
 * @param buffer Data buffer
 * @param block_no The block we want to save into our buffer
 * @return (int) -1 for error, 1 for ok
 */
int read_block(int * fd, void * buffer, int block_no) {
    if (fd == NULL || buffer == NULL) {
        pprintf("Invalid parameters provided [read_block]");
        return -1;
    }

    if (*fd < 0) {
        pprintf("Invalid file descriptor provided [read_block]");
        return -1;
    }

    if (block_no < 0 || block_no > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block_no provided [read_block]");
        return -1;
    }

    if (lseek(*fd, (block_no * BLOCK_SIZE), SEEK_SET) == -1) {
        pprintf("Unable to seek in file [read_block]");
        perror("lseek");
        return -1;
    }

    if (read(*fd, buffer, BLOCK_SIZE) == -1) {
        pprintf("Unable to read block [read_block]");
        perror("read");
        return -1;
    }

    return 1;
}

/**
 * @brief Writes a block of data, present in buffer, to the file provided by the fd
 * 
 * @param fd File descriptor
 * @param buffer Data buffer
 * @param block_no The block we want to write to
 * @return (int) -1 for error, 1 for ok
 */
int write_block(int * fd, void * buffer, int block_no) {
    if (fd == NULL || buffer == NULL) {
        pprintf("Invalid parameters provided [write_block]");
        return -1;
    }

    if (*fd < 0) {
        pprintf("Invalid file descriptor provided [write_block]");
        return -1;
    }

    if (block_no < 0 || block_no > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block_no provided [write_block]");
        return -1;
    }

    if (lseek(*fd, (block_no * BLOCK_SIZE), SEEK_SET) == -1) {
        pprintf("Unable to seek in file [write_block]");
        perror("lseek");
        return -1;
    }

    if (write(*fd, buffer, BLOCK_SIZE) == -1) {
        pprintf("Unable to write block [write_block]");
        perror("read");
        return -1;
    }

    return 1;    
}

/**
 * @brief Cleans a particular block, replacing all bytes with null-terminator ('\0')
 * 
 * @param fd File descriptor
 * @param block_no The block we want to clean
 * @return (int) -1 for error, 1 for ok 
 */
int clean_block(int * fd, int block_no) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [clean_block]");
        return -1;
    }

    if (block_no < 0 || block_no > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block_no provided [clean_block]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [clean_block]");
        perror("malloc");
        return -1;
    }

    memset(block_buffer, '\0', BLOCK_SIZE);

    if (write_block(fd, (void *) block_buffer, block_no) == -1) {
        pprintf("Unable to write block [clean_block]");
        free(block_buffer);
        return -1;
    }

    free(block_buffer);

    return 1;
}

/**
 * @brief Cleans all the blocks, replacing all bytes with the null-terminator '\0'
 * 
 * @param fd File descriptor
 * @return (int) -1 for error, 1 for ok
 */
int clean_all_blocks(int * fd) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [clean_all_blocks]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [clean_all_blocks]");
        perror("malloc");
        return -1;
    }

    memset(block_buffer, '\0', BLOCK_SIZE);

    int i;
    
    for (i = 0; i < TOTAL_NO_OF_BLOCKS; i++) {
        if (write_block(fd, (void *) block_buffer, i) == -1) {
            pprintf("Unable to write block [clean_all_blocks]");
            free(block_buffer);
            return -1;
        }
    }

    free(block_buffer);

    return 1;
}
