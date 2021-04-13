#include <stdio.h>
#include "../include/globals.h"
#include "../include/inodes.h"

int main () {

    printf("BLOCK_SIZE = %d BYTES\n", BLOCK_SIZE);
    printf("DISK_SIZE = %d BYTES\n", DISK_SIZE);
    printf("NO_OF_BLOCKS = %d BLOCKS\n", NO_OF_BLOCKS);
    printf("NO_OF_INODE_BLOCKS = %d BLOCKS\n", NO_OF_INODE_BLOCKS);
    printf("GET_NO_OF_INODES = %d INODES\n", GET_NO_OF_INODES(sizeof(inode)));

    return 0;
}