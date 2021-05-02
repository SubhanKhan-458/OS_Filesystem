/**
 * Creates, maintains and initializes the Superblock
 */

#include <stdio.h>

#include "../include/superblock.h"

void initialize_superblock(int * descriptor) {
    superblock * SUPER_BLOCK = (superblock *) malloc(sizeof(superblock));

    if (!SUPER_BLOCK) {
        perror("malloc");
        exit(1);
    }

    SUPER_BLOCK->magic_number = 0x534E5300; // SNS0
    SUPER_BLOCK->total_blocks = NO_OF_BLOCKS;
    SUPER_BLOCK->inode_blocks = NO_OF_INODE_BLOCKS;
    SUPER_BLOCK->no_of_inodes = GET_NO_OF_INODES(sizeof(inode));

    writeBlock(descriptor, SUPER_BLOCK, 0);

    free(SUPER_BLOCK);
};
