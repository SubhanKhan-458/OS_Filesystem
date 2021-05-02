/**
 * Creates and maintains inodes
 */

#include <stdio.h>
#include <string.h>

#include "../include/block.h"
#include "../include/inodes.h"
#include "../include/olt.h"

// only run on filesystem registration (only once)
void initialize_inode_blocks(int * device_descriptor) {
    if (*device_descriptor < 0) {
        return;
    }

    int i, inodeBlockIndex = INODE_BLOCK_NO;
    const ssize_t NO_OF_INODES_PER_BLOCK = (BLOCK_SIZE / sizeof(inode));
    // inode inode_buffer = {
    //     .size = 0,
    //     .type = IS_EMPTY,
    // };
    inode * inode_block_buffer = (inode *) malloc(sizeof(inode) * NO_OF_INODES_PER_BLOCK);

    for (i = 0; i < NO_OF_INODES_PER_BLOCK; i++) {
        // memcpy(inode_block_buffer + (i * sizeof(inode)), &inode_buffer, sizeof(inode));
        inode_block_buffer[i].size = 0;
        inode_block_buffer[i].type = IS_EMPTY;
    }

    for (i = 0; i < NO_OF_INODE_BLOCKS; i++) {
        if (writeBlock(device_descriptor, (void *) inode_block_buffer, (inodeBlockIndex++)) == 0) {
            printf("Unable to write block\n");
            return;
        }
    }

    free(inode_block_buffer);
}

void write_inode(int * device_descriptor, inode * buffer, int inode_index) {
    if (*device_descriptor < 0) {
        return;
    }

    int block_no = get_inode_block_no(inode_index);
    char * inode_block_buffer = (char *) malloc(BLOCK_SIZE);
    int index_in_block = inode_index - ((GET_INODES_PER_BLOCK(sizeof(inode))) * block_no);

    if (block_no > NO_OF_INODE_BLOCKS || block_no < 0 || index_in_block < 0 || index_in_block > (NO_OF_INODES - 1)) {
        free(inode_block_buffer);
        return;
    }

    if (readBlock(device_descriptor, (void *) inode_block_buffer, block_no) == 0) {
        free(inode_block_buffer);
        return; // would cause mem leak on return here
    }

    memcpy(inode_block_buffer + (index_in_block * sizeof(inode)), buffer, sizeof(inode));

    if (writeBlock(device_descriptor, (void *) inode_block_buffer, block_no) == 0) {
        free(inode_block_buffer);
        return;
    }

    INODE_BITMAP.bitmap[inode_index] = '1';
    free(inode_block_buffer);
}

int get_inode_block_no(int inode_index) {
    const ssize_t NO_OF_INODES_PER_BLOCK = GET_INODES_PER_BLOCK(sizeof(inode));

    return (inode_index == 0 ? 0 : (inode_index / NO_OF_INODES_PER_BLOCK));
}

// int main () {
//     int * fd = (int *) malloc(sizeof(int));
//     openDisk("../foo.txt", fd);
//     initialize_inode_blocks(fd);

//     char * block = (char *) malloc(BLOCK_SIZE);
//     inode * buff = (inode *) malloc(sizeof(inode));

//     buff->type = IS_A_DIRECTORY;
//     buff->size = 1024;

//     write_inode(fd, buff, 0); // 0
//     write_inode(fd, buff, 45); // 0 
//     write_inode(fd, buff, 46); // 1
//     write_inode(fd, buff, 9015); // 196
//     write_inode(fd, buff, 500); // 10

//     int i = 0, j = 0, k = 0;
//     for (i = 0; i < NO_OF_INODE_BLOCKS; i++) {
//         readBlock(fd, (void *) block, k++);
//         for (j = 0; j < 46; j++) {
//             memcpy(buff, block + (j * sizeof(inode)), sizeof(inode));
//             if (buff->size != 0 || buff->type != 2) {
//                 printf("ERROR HERE!\n");
//                 printf("Size = %d | Type = %d | Block = %d | Index = %d\n", buff->size, buff->type, i, j);
//             }
//             // printf("Size = %d | Type = %d\n", buff->size, buff->type);
//         }
//     }

//     free(buff);
//     free(block);
    
//     closeDisk(fd);

//     free(fd);

//     return 0;
// }