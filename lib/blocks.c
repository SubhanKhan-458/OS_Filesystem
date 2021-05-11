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
