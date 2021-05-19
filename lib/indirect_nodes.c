/**
 * @file indirect_nodes.c
 * @author Shaheer Ahmed (k190233@nu.edu.pk) & Noman Vadsariya & Subhan Khan
 * @brief Provides methods for managing indirect nodes for our FS
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/globals.h"

/**
 * @brief Initializes the indirect node blocks in the block device
 * 
 * @param fd File descriptor
 * @return (int) -1 for error, 1 for ok 
 */
int initialize_indirect_node_blocks(int * fd) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [initialize_indirect_node_blocks]");
        return -1;
    }

    indirect_node temp;

    // 0 indicates empty, subtract 1 from pointer to get the actual pointer
    memset(temp.pointers, 0, NO_OF_DIRECT_INDEXES * sizeof(int32_t));

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [initialize_indirect_node_blocks]");
        perror("malloc");
        return -1;
    }

    int i;

    // ensures that any remaining bytes in a block are always null-terminators
    memset(block_buffer, '\0', BLOCK_SIZE);

    // create a block filled with empty indirect nodes
    for (i = 0; i < NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE); i++) {
        memcpy(block_buffer + (i * SIZEOF_INDIRECT_NODE), &temp, SIZEOF_INDIRECT_NODE);
    }

    // write indirect node blocks starting from the first indirect node block index till data block index
    // (it is the same as total no of indirect node blocks)
    for (i = INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i < DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE); i++) {
        if (write_block(fd, (void *) block_buffer, i) == -1) {
            pprintf("Unable to write block [initialize_indirect_node_blocks]");
            free(block_buffer);
            return -1;
        }
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Updates a pointer to data block in the specified indirect node
 * 
 * Data block indexes are absolute, i.e. they begin from DATA_BLOCKS_INDEX_NO
 * 
 * @param fd File descriptor
 * @param indirect_node_index Index of the target indirect node
 * @param pointer_index Index of the pointer to update
 * @param data_block_index Index of the data block being assigned to this indirect node
 * @return (int) -1 for error, 1 for ok 
 */
int add_pointer_to_indirect_node(int * fd, int indirect_node_index, int pointer_index, int data_block_index) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [add_pointer_to_indirect_node]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INDIRECT_NODES - 1) is equal to the TOTAL_NO_OF_INDIRECT_NODES,
    // hence when >= is used, not just >
    if (indirect_node_index < 0 || indirect_node_index >= TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE)) {
        pprintf("Invalid indirect node index provided [add_pointer_to_indirect_node]");
        return -1;
    }

    // Comparing indexes, hence >=, since (N - 1) would be the max index
    if (pointer_index < 0 || pointer_index >= (NO_OF_DIRECT_INDEXES)) {
        pprintf("Invalid pointer index provided [add_pointer_to_indirect_node]");
        return -1;
    }

    int block_no = (int) (indirect_node_index / NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE));
    int indirect_node_blocks_index = INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY);
    if ((block_no + indirect_node_blocks_index) < 0 || (block_no + indirect_node_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block no calculated [add_pointer_to_indirect_node]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [add_pointer_to_indirect_node]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, indirect_node_blocks_index + block_no) == -1) {
        pprintf("Unable to read block [add_pointer_to_indirect_node]");
        free(block_buffer);
        return -1;
    }

    int offset = (indirect_node_index - (block_no * NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE))) * SIZEOF_INDIRECT_NODE;

    indirect_node temp;
    memcpy(&temp, block_buffer + offset, SIZEOF_INDIRECT_NODE);
    temp.pointers[pointer_index] = data_block_index;
    memcpy(block_buffer + offset, &temp, SIZEOF_INDIRECT_NODE);

    if (write_block(fd, (void *) block_buffer, indirect_node_blocks_index + block_no) == -1) {
        pprintf("Unable to write block [add_pointer_to_indirect_node]");
        free(block_buffer);
        return -1;
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Resets a pointer in an indirect node back to 0
 * 
 * @param fd File descriptor
 * @param indirect_node_index The index of target indirect node
 * @param pointer_index The index of the pointer to reset
 * @return (int) -1 for error, 1 for ok 
 */
int remove_pointer_from_indirect_node(int * fd, int indirect_node_index, int pointer_index) {
    if (add_pointer_to_indirect_node(fd, indirect_node_index, pointer_index, 0) == -1) {
        pprintf("Unable to remove pointer index [remove_pointer_from_indirect_node]");
        return -1;
    }
}

/**
 * @brief Writes an indirect node to the block device file
 * 
 * The indirect node's indexing begins from 0 till "TOTAL_NO_OF_INDIRECT_NODES - 1"
 * 
 * @param fd File descriptor
 * @param buffer The indirect node buffer, it contains the indirect node data we want to write
 * @param inode_index The indirect node index we want to write to
 * @return (int) -1 for error, 1 for ok 
 */
int write_indirect_node(int * fd, indirect_node * buffer, int indirect_node_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        pprintf("Invalid parameters provided [write_indirect_node]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INDIRECT_NODES - 1) is equal to the TOTAL_NO_OF_INDIRECT_NODES,
    // hence when >= is used, not just >
    if (indirect_node_index < 0 || indirect_node_index >= TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE)) {
        pprintf("Invalid indirect node index provided [write_indirect_node]");
        return -1;
    }

    int block_no = (int) (indirect_node_index / NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE));
    int indirect_node_blocks_index = INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY);
    if ((block_no + indirect_node_blocks_index) < 0 || (block_no + indirect_node_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block no calculated [write_indirect_node]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [write_indirect_node]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, indirect_node_blocks_index + block_no) == -1) {
        pprintf("Unable to read block [write_indirect_node]");
        free(block_buffer);
        return -1;
    }

    int offset = (indirect_node_index - (block_no * NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE))) * SIZEOF_INDIRECT_NODE;
    memcpy(block_buffer + offset, buffer, SIZEOF_INDIRECT_NODE);

    if (write_block(fd, (void *) block_buffer, indirect_node_blocks_index + block_no) == -1) {
        pprintf("Unable to write block [write_indirect_node]");
        free(block_buffer);
        return -1;
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Saves the targetted indirect node's data to the indirect node buffer
 * 
 * @param fd File descriptor
 * @param buffer The indirect node buffer, where we would save the data
 * @param indirect_node_index The target indirect node's index
 * @return (int) -1 for error, 1 for ok 
 */
int read_indirect_node(int * fd, indirect_node * buffer, int indirect_node_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        pprintf("Invalid parameters provided [read_indirect_node]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INODES - 1) is equal to the TOTAL_NO_OF_INODES,
    // hence when >= is used, not just >
    if (indirect_node_index < 0 || indirect_node_index >= TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE)) {
        pprintf("Invalid indirect node index provided [read_indirect_node]");
        return -1;
    }

    int block_no = (int) (indirect_node_index / NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE));
    int indirect_node_blocks_Index = INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY);
    if ((block_no + indirect_node_blocks_Index) < 0 || (block_no + indirect_node_blocks_Index) > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block no calculated [read_indirect_node]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [read_indirect_node]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, indirect_node_blocks_Index + block_no) == -1) {
        pprintf("Unable to read block [read_indirect_node]");
        free(block_buffer);
        return -1;
    }

    int offset = (indirect_node_index - (block_no * NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE))) * SIZEOF_INDIRECT_NODE;
    memcpy(buffer, block_buffer + offset, SIZEOF_INDIRECT_NODE);

    free(block_buffer);
    return 1;
}

/**
 * @brief Cleans a targetted indirect node, resetting all attributes to the "empty state"
 * 
 * "empty state" is the initial state, where all pointers have the value 0
 * 
 * @param fd File descriptor
 * @param indirect_node_index The targetted indirect node's index
 * @return (int) -1 for error, 1 for ok 
 */
int clean_indirect_node(int * fd, int indirect_node_index) {
    indirect_node temp;

    // 0 indicates empty, subtract 1 from pointer to get the actual pointer
    memset(temp.pointers, 0, NO_OF_DIRECT_INDEXES * sizeof(int32_t));

    if (write_indirect_node(fd, &temp, indirect_node_index) == -1) {
        pprintf("Unable to clean indirect node [clean_indirect_node]");
        return -1;
    }

    return 1;
}

/**
 * @brief Dumps an indirect node's data to stdout
 * 
 * @param buffer The indirect node whose data we want to dump
 */
void dump_indirect_node(indirect_node * buffer) {
    if (buffer == NULL) {
        printf("Invalid parameters provided [dump_indirect_node]");
        return;
    }

    printf("--------------------------------------------\n");
    printf("INDIRECT NODE\n");
    printf("--------------------------------------------\n");
    
    int i;
    printf("direct indexes = ");
    for (i = 0; i < NO_OF_DIRECT_INDEXES; i++) {
        printf("[%d] ", buffer->pointers[i]);
    }
    
    printf("\n");
    printf("--------------------------------------------\n");

    return;
}
