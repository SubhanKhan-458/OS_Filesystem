/**
 * Creates and manages Object Location Table.
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "../include/olt.h"
#include "../include/block.h"

// device_descriptor = diskID
void initialize_inode_bitmap(int * device_descriptor) {
    if (*device_descriptor < 0) {
        return;
    }

    int i = 0, bitmapIndex = 0, inodeBlockIndex = INODE_BLOCK_NO;

    inode * inode_buffer = (inode *) malloc(sizeof(inode));
    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    off_t block_offset;

    // const ssize_t NO_OF_INODES_PER_BLOCK = (BLOCK_SIZE / sizeof(inode));

    if (!inode_buffer || !block_buffer) {
        perror("malloc");
        return; // make a safe exit function later to clean up everything before exiting
    }

    while (i <= NO_OF_INODE_BLOCKS) {
        if (((BLOCK_SIZE - block_offset) < sizeof(inode)) || (inodeBlockIndex == INODE_BLOCK_NO)) {
            if (readBlock(device_descriptor, (void *) block_buffer, (inodeBlockIndex++)) == 0) {
                printf("Unable to read block\n");
                return;
            }
            block_offset = 0;
            i++; // might not read the last block
        }

        // exception check?
        memcpy(inode_buffer, block_buffer + (block_offset), sizeof(inode));
        INODE_BITMAP.bitmap[bitmapIndex++] = (inode_buffer->size > 0 ? '1' : '0');
        block_offset += sizeof(inode);
    }

    free(inode_buffer);
    free(block_buffer);
}

// Replace INODE_BLOCK_NO with 0 while testing
// int main () {
//     int * fd = (int *) malloc(sizeof(int));
//     openDisk("../foo.txt", fd);
//     // initialize_inode_blocks(fd);
//     initialize_inode_bitmap(fd);

//     int i = 0;
//     for (i = 0; i < NO_OF_INODES; i++) {
//         if (INODE_BITMAP.bitmap[i] == '1') {
//             printf("%c [%d]\n", INODE_BITMAP.bitmap[i], i);
//         }
//     }

//     printf("\n");

//     closeDisk(fd);

//     free(fd);

//     return 0;
// }