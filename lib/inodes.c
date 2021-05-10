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

    int i, j, inodeBlockIndex = INODE_BLOCK_NO;
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
        for (j = 0; j < (DIRECT_BLOCKS + INDIRECT_DEPTH); j++) {
            inode_block_buffer[i].index_pointers[j] = -1;
        }
    }

    for (i = 0; i < NO_OF_INODE_BLOCKS; i++) {
        if (writeBlock(device_descriptor, (void *) inode_block_buffer, (inodeBlockIndex++)) == 0) {
            printf("Unable to write block\n");
            return;
        }
    }

    free(inode_block_buffer);
}

// in buffer, set index_pointers to -1
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

void reset_inode(int * device_descriptor, int inode_index) {
    if (*device_descriptor < 0) {
        return;
    }

    inode * buffer = (inode *) malloc(sizeof(inode));

    buffer->size = 0;
    buffer->type = IS_EMPTY;

    write_inode(device_descriptor, buffer, inode_index);
}

void read_inode(int * device_descriptor, inode * buffer, int inode_index) {
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

    free(inode_block_buffer);
}

int get_inode_block_no(int inode_index) {
    const ssize_t NO_OF_INODES_PER_BLOCK = GET_INODES_PER_BLOCK(sizeof(inode));

    return (inode_index == 0 ? 0 : (inode_index / NO_OF_INODES_PER_BLOCK));
}

void initialize_indirect_node_blocks(int * device_descriptor) {
    if (*device_descriptor < 0) {
        return;
    }

    int i, indirect_nodeBlockIndex = INDEX_BLOCK_NO;
    const ssize_t NO_OF_INDIRECT_NODES_PER_BLOCK = (BLOCK_SIZE / sizeof(single_indirect_block));
    single_indirect_block * indirect_node_block_buffer = (single_indirect_block *) malloc(sizeof(single_indirect_block) * NO_OF_INDIRECT_NODES_PER_BLOCK);

    for (i = 0; i < NO_OF_INDIRECT_NODES_PER_BLOCK; i++) {
        memset(indirect_node_block_buffer[i].index_pointers, -1, DIRECT_BLOCKS);
    }

    for (i = 0; i < NO_OF_INDIRECT_NODES_PER_BLOCK; i++) {
        if (writeBlock(device_descriptor, (void *) indirect_node_block_buffer, (indirect_nodeBlockIndex++)) == 0) {
            printf("Unable to write block\n");
            return;
        }
    }

    free(indirect_node_block_buffer);
}

int get_indirect_block_no(int indirect_node_index) {
    const ssize_t NO_OF_INDIRECT_NODES_PER_BLOCK = GET_INDIRECT_NODES_PER_BLOCK(sizeof(single_indirect_block));

    return (indirect_node_index == 0 ? 0 : (indirect_node_index / NO_OF_INDIRECT_NODES_PER_BLOCK));
}

void write_indirect_node(int * device_descriptor, single_indirect_block * buffer, int indirect_node_index) {
        if (*device_descriptor < 0) {
        return;
    }

    int block_no = get_indirect_block_no(indirect_node_index);
    char * indirect_block_buffer = (char *) malloc(BLOCK_SIZE);
    int index_in_block = indirect_node_index - ((GET_INDIRECT_NODES_PER_BLOCK(sizeof(single_indirect_block))) * block_no);

    if (block_no > NO_OF_INDEX_NODE_BLOCKS || block_no < 0 || index_in_block < 0 || index_in_block > (GET_INDIRECT_NODES_PER_BLOCK(sizeof(single_indirect_block)))) {
        free(indirect_block_buffer);
        return;
    }

    if (readBlock(device_descriptor, (void *) indirect_block_buffer, block_no) == 0) {
        free(indirect_block_buffer);
        return; // would cause mem leak on return here
    }

    memcpy(indirect_block_buffer + (index_in_block * sizeof(single_indirect_block)), buffer, sizeof(single_indirect_block));

    if (writeBlock(device_descriptor, (void *) indirect_block_buffer, block_no) == 0) {
        free(indirect_block_buffer);
        return;
    }

    INDEX_BLOCK_BITMAP.bitmap[indirect_node_index] = '1';
    free(indirect_block_buffer);
}

void read_indirect_node(int * device_descriptor, single_indirect_block * buffer, int indirect_node_index) {
    if (*device_descriptor < 0) {
        return;
    }

    int block_no = get_indirect_block_no(indirect_node_index);
    char * indirect_node_block_buffer = (char *) malloc(BLOCK_SIZE);
    int index_in_block = indirect_node_index - ((GET_INDIRECT_NODES_PER_BLOCK(sizeof(single_indirect_block))) * block_no);

    if (block_no > NO_OF_INDEX_NODE_BLOCKS || block_no < 0 || index_in_block < 0 || index_in_block > (GET_INDIRECT_NODES_PER_BLOCK(sizeof(single_indirect_block)))) {
        free(indirect_node_block_buffer);
        return;
    }

    if (readBlock(device_descriptor, (void *) indirect_node_block_buffer, block_no) == 0) {
        free(indirect_node_block_buffer);
        return; // would cause mem leak on return here
    }

    memcpy(indirect_node_block_buffer + (index_in_block * sizeof(single_indirect_block)), buffer, sizeof(single_indirect_block));

    free(indirect_node_block_buffer);
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