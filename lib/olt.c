/**
 * @file olt.c [Object Location Table]
 * @author Shaheer Ahmed (k190233@nu.edu.pk)
 * @brief Provides methods for object location tables for inodes, indirect nodes etc.
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/globals.h"

int initialize_bitmaps(int * fd) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [initialize_bitmaps]");
        return -1;
    }

    inode temp_inode;
    indirect_node temp_indirect_node;

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    char * data_block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL || data_block_buffer == NULL) {
        pprintf("Unable to allocate memory [initialize_bitmaps]");
        perror("malloc");
        return -1;
    }

    int i, j, k, l, bitmap_index = 0, data_block_index, indirect_node_index;
    int data_block_index_no = DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE);

    for (i = INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i < INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i++) {
        if (read_block(fd, (void *) block_buffer, i) == -1) {
            pprintf("Unable to read block [initialize_bitmaps]");
            free(block_buffer);
            free(data_block_buffer);
            return -1;
        }

        for (j = 0; j < NO_OF_INODES_PER_BLOCK(SIZEOF_INODE); j++) {
            memcpy(&temp_inode, block_buffer + (j * SIZEOF_INODE), SIZEOF_INODE);
            inodes_bitmap[bitmap_index++] = (temp_inode.type != IS_EMPTY_INODE ? 1 : 0);
            // fill in the indirect_node and data_block bitmap through the inode pointers
            if (temp_inode.type != IS_EMPTY_INODE) {
                for (k = 0; k < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); k++) {
                    if (k < NO_OF_DIRECT_INDEXES) {
                        data_block_index = temp_inode.pointers[k];
                        if (data_block_index != 0) {
                            // should point to absolute data_blocks
                            // not relative (relative => start from 0)
                            // since it is absolute, we need to subtract DATA_BLOCK_INDEX_NO
                            // to get the bitmap index for it
                            data_blocks_bitmap[data_block_index - data_block_index_no] = mark_data_block(fd, data_block_buffer, data_block_index);
                        }
                    } else {
                        // this marks the start of indirect node indexes
                        indirect_node_index = temp_inode.pointers[k];
                        if (indirect_node_index != 0) {
                            // indirect_nodes are relative, since 0 signifies -1
                            // we need to subtract 1 to get the actual index
                            indirect_nodes_bitmap[(indirect_node_index - 1)] = 1;

                            if (read_indirect_node(fd, &temp_indirect_node, (indirect_node_index - 1)) == -1) {
                                pprintf("Unable to read indirect node [initialize_bitmaps]");
                                free(block_buffer);
                                free(data_block_buffer);
                                return -1;
                            }

                            for (l = 0; l < NO_OF_DIRECT_INDEXES; l++) {
                                data_block_index = temp_indirect_node.pointers[l];
                                if (data_block_index != 0) {
                                    data_blocks_bitmap[data_block_index - data_block_index_no] = mark_data_block(fd, data_block_buffer, data_block_index);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    free(block_buffer);
    return 1;
}

// 0 for empty, 1 for full, 2 for half-full (has some remaining space)
int mark_data_block(int * fd, char * block_buffer, int data_block_index) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [mark_data_block]");
        return -1;
    }

    int bytes_read;

    if (data_block_index < DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) || data_block_index > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid data block index provided [mark_data_block]");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, data_block_index) == -1) {
        pprintf("Unable to read block [mark_data_block]");
        free(block_buffer);
        return -1;
    }

    // read till null-terminator is found, or till BLOCK_SIZE is reached
    for (bytes_read = 0; (block_buffer[bytes_read] != '\0') && (bytes_read < BLOCK_SIZE); bytes_read++);

    if (bytes_read == 0) {
        return 0;
    }

    // if last byte of our block is null-terminator
    // we also regard that as data-block being full
    if (block_buffer[bytes_read] == '\0' && (bytes_read + 1 == BLOCK_SIZE)) {
        return 1;
    }

    return (bytes_read == BLOCK_SIZE ? 1 : 2);
}

int get_free_inode_index() {
    int i;
    for (i = 0; i < TOTAL_NO_OF_INODES(SIZEOF_INODE); i++) {
        if (inodes_bitmap[i] == 0) {
            return i;
        }
    }

    pprintf("No free inode available [get_free_inode_index]");
    return -1;
}

int set_inode_bitmap_value(int inode_index, int value) {
    if (inode_index < 0 || inode_index > TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid inode index provided [set_inode_bitmap_value]");
        return -1;
    }

    inodes_bitmap[inode_index] = value;
}

int get_free_indirect_node_index() {
    int i;
    for (i = 0; i < TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE); i++) {
        if (indirect_nodes_bitmap[i] == 0) {
            return i;
        }
    }

    pprintf("No free indirect node available [get_free_indirect_node_index]");
    return -1;
}

int set_indirect_node_bitmap_value(int indirect_node_index, int value) {
    if (indirect_node_index < 0 || indirect_node_index > TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE)) {
        pprintf("Invalid indirect node index provided [set_indirect_node_bitmap_value]");
        return -1;
    }

    indirect_nodes_bitmap[indirect_node_index] = value;
}

int get_free_data_block_index() {
    int i;
    for (i = 0; i < TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE); i++) {
        if (data_blocks_bitmap[i] == 0) {
            return (DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) + i);
        }
    }

    pprintf("No free data block available [get_free_data_block_index]");
    return -1;
}

int get_data_block_bitmap_value(int data_block_index) {
    if (data_block_index == 0) {
        return 0;
    }

    int i = (data_block_index - DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE));
    if (i < 0 || i > TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE)) {
        pprintf("Invalid data_block_index provided [get_data_block_bitmap_value]");
        return -1;
    }

    return data_blocks_bitmap[i];
}

int set_data_block_bitmap_value(int data_block_index, int value) {
    int i = (data_block_index - DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE));
    if (i < 0 || i > TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE)) {
        pprintf("Invalid data_block_index provided [set_data_block_bitmap_value]");
        return -1;
    }

    data_blocks_bitmap[i] = value;
    return 1;
}

void dump_inode_bitmap() {
    int i;

    printf("--------------------------------------------\n");
    printf("INODE BITMAP\n");
    printf("--------------------------------------------\n");

    for (i = 0; i < TOTAL_NO_OF_INODES(SIZEOF_INODE); i++) {
        printf("{[%d] %d} ", i, inodes_bitmap[i]);
    }

    printf("\n");
    printf("--------------------------------------------\n");

    return;
}

void dump_indirect_node_bitmap() {
    int i;

    printf("--------------------------------------------\n");
    printf("INDIRECT NODE BITMAP\n");
    printf("--------------------------------------------\n");

    for (i = 0; i < TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE); i++) {
        printf("{[%d] %d} ", i, indirect_nodes_bitmap[i]);
    }

    printf("\n");
    printf("--------------------------------------------\n");

    return;
}

void dump_data_block_bitmap() {
    int i;

    printf("--------------------------------------------\n");
    printf("DATA BLOCK BITMAP\n");
    printf("--------------------------------------------\n");

    for (i = 0; i < TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE); i++) {
        printf("{[%d] %d} ", i, data_blocks_bitmap[i]);
    }

    printf("\n");
    printf("--------------------------------------------\n");

    return;
}
