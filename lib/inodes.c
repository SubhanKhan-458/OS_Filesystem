#include "../include/globals.h"

/**
 * @brief Initializes the inode blocks section of the block device
 * 
 * Initializes all inodes to "empty states", i.e.
 * ->size = 0,
 * ->type = IS_EMPTY_INODE,
 * ->pointers (each one) = 0,
 * 
 * The 0 in pointers indicates empty, the actual indexes can be 
 * obtained by subtracting 1 from the pointer value (which would be > 0)
 * (Pointer Value = 1, Actual Index = 1 - 1 => 0)
 * 
 * @param fd File descriptor
 * @return (int) -1 for error, 1 for ok  
 */
int initialize_inode_blocks(int * fd) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [initialize_inode_blocks]");
        return -1;
    }

    inode temp = {
        .size = 0,
        .type = IS_EMPTY_INODE,
        .uid = 0,
        .gid = 0,
        .last_accessed = 0,
        .last_changed = 0,
        .created_at = 0,
    };

    // 0 indicates empty, subtract 1 from pointer to get the actual pointer
    memset(temp.pointers, 0, (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES) * sizeof(int32_t));

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [initialize_inode_blocks]");
        perror("malloc");
        return -1;
    }

    int i;

    // ensures that any remaining bytes in a block are always null-terminators
    memset(block_buffer, '\0', BLOCK_SIZE);

    // create a block filled with empty inodes
    for (i = 0; i < NO_OF_INODES_PER_BLOCK(SIZEOF_INODE); i++) {
        memcpy(block_buffer + (i * SIZEOF_INODE), &temp, SIZEOF_INODE);
    }

    // write inode blocks starting from the first inode block index till indirect node block index
    // (it is the same as total no of inode blocks)
    for (i = INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i < INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i++) {
        if (write_block(fd, (void *) block_buffer, i) == -1) {
            pprintf("Unable to write block [initialize_inode_blocks]");
            free(block_buffer);
            return -1;
        }
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Writes an inode to the block device file
 * 
 * The inode blocks section's starting block no can be 
 * obtained by the "INODE_BLOCKS_INDEX_NO" macro
 * 
 * The inode's indexing begins from 0 till "TOTAL_NO_OF_INODES - 1"
 * 
 * @param fd File descriptor
 * @param buffer The inode buffer, it contains the inode data we want to write
 * @param inode_index The inode index we want to write to
 * @return (int) -1 for error, 1 for ok 
 */
int write_inode(int * fd, inode * buffer, int inode_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        pprintf("Invalid parameters provided [write_inode]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INODES - 1) is equal to the TOTAL_NO_OF_INODES,
    // hence when >= is used, not just >
    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid inode index provided [write_inode]");
        return -1;
    }

    int block_no = (int) (inode_index / NO_OF_INODES_PER_BLOCK(SIZEOF_INODE));
    int inode_blocks_index = INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY);
    if ((block_no + inode_blocks_index) < 0 || (block_no + inode_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block no calculated [write_inode]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [write_inode]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, inode_blocks_index + block_no) == -1) {
        pprintf("Unable to read block [write_inode]");
        free(block_buffer);
        return -1;
    }

    int offset = (inode_index - (block_no * NO_OF_INODES_PER_BLOCK(SIZEOF_INODE))) * SIZEOF_INODE;
    memcpy(block_buffer + offset, buffer, SIZEOF_INODE);

    if (write_block(fd, (void *) block_buffer, inode_blocks_index + block_no) == -1) {
        pprintf("Unable to write block [write_inode]");
        free(block_buffer);
        return -1;
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Saves the targetted inode's data to the inode buffer
 * 
 * @param fd File descriptor
 * @param buffer The inode buffer, where we would save the data
 * @param inode_index The target inode's index
 * @return (int) -1 for error, 1 for ok 
 */
int read_inode(int * fd, inode * buffer, int inode_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        pprintf("Invalid parameters provided [read_inode]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INODES - 1) is equal to the TOTAL_NO_OF_INODES,
    // hence when >= is used, not just >
    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid inode index provided [read_inode]");
        return -1;
    }

    int block_no = (int) (inode_index / NO_OF_INODES_PER_BLOCK(SIZEOF_INODE));
    int inode_blocks_index = INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY);
    if ((block_no + inode_blocks_index) < 0 || (block_no + inode_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        pprintf("Invalid block no calculated [read_inode]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [read_inode]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, inode_blocks_index + block_no) == -1) {
        pprintf("Unable to read block [read_inode]");
        free(block_buffer);
        return -1;
    }

    int offset = (inode_index - (block_no * NO_OF_INODES_PER_BLOCK(SIZEOF_INODE))) * SIZEOF_INODE;
    memcpy(buffer, block_buffer + offset, SIZEOF_INODE);

    free(block_buffer);
    return 1;
}

/**
 * @brief Cleans a targetted inode, resetting all attributes to the "empty state"
 * 
 * "empty state" has been defined in detail in "initialize_inode_blocks"
 * 
 * @param fd File descriptor
 * @param inode_index The targetted inode's index
 * @return (int) -1 for error, 1 for ok 
 */
int clean_inode(int * fd, int inode_index) {
    inode temp = {
        .size = 0,
        .type = IS_EMPTY_INODE,
        .uid = 0,
        .gid = 0,
        .last_accessed = 0,
        .last_changed = 0,
        .created_at = 0,
    };

    // 0 indicates empty, subtract 1 from pointer to get the actual pointer
    memset(temp.pointers, 0, (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES) * sizeof(int32_t));

    if (write_inode(fd, &temp, inode_index) == -1) {
        pprintf("Unable to clean inode [clean_inode]");
        return -1;
    }

    return 1;
}

/**
 * @brief Dumps an inode's data to stdout
 * 
 * @param buffer The inode whose data we want to dump
 */
void dump_inode(inode * buffer) {
    if (buffer == NULL) {
        printf("Invalid parameters provided [dump_inode]");
        return;
    }

    char type_empty_msg[] = "EMPTY";
    char type_dir_msg[] = "DIRECTORY";
    char type_file_msg[] = "FILE";

    printf("--------------------------------------------\n");
    printf("INODE\n");
    printf("--------------------------------------------\n");
    printf("size = %d BYTES,\n", buffer->size);
    printf("type = %s,\n", (buffer->type == IS_EMPTY_INODE ? type_empty_msg : buffer->type == IS_DIR_INODE ? type_dir_msg : type_file_msg));
    printf("user id = %d,\n", buffer->uid);
    printf("group id = %d,\n", buffer->gid);
    printf("last accessed = %ld,\n", buffer->last_accessed);
    printf("last changed = %ld,\n", buffer->last_changed);
    printf("created at = %ld,\n", buffer->created_at);
    
    int i;
    printf("direct indexes = ");
    for (i = 0; i < NO_OF_DIRECT_INDEXES; i++) {
        printf("[%d] ", buffer->pointers[i]);
    }
    
    printf("\n");
    printf("indirect indexes = ");
    
    for (; i < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); i++) {
        printf("[%d] ", buffer->pointers[i]);
    }
    
    printf("\n");
    printf("--------------------------------------------\n");

    return;
}

int initialize_inode_bitmap(int * fd) {
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [initialize_inode_bitmap]");
        return -1;
    }

    inode temp_inode;
    indirect_node temp_indirect_node;

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [initialize_inode_bitmap]");
        perror("malloc");
        return -1;
    }

    int i, j, k, l, bitmap_index = 0, data_block_index, indirect_node_index;
    int data_block_index_no = DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE);

    for (i = INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i < INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i++) {
        if (read_block(fd, (void *) block_buffer, i) == -1) {
            pprintf("Unable to read block [initialize_inode_bitmap]");
            free(block_buffer);
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
                            data_blocks_bitmap[data_block_index - data_block_index_no] = 1;
                        }
                    } else {
                        // this marks the start of indirect node indexes
                        indirect_node_index = temp_inode.pointers[k];
                        if (indirect_node_index != 0) {
                            // indirect_nodes are relative, since 0 signifies -1
                            // we need to subtract 1 to get the actual index
                            indirect_nodes_bitmap[(indirect_node_index - 1)] = 1;

                            if (read_indirect_node(fd, &temp_indirect_node, (indirect_node_index - 1)) == -1) {
                                pprintf("Unable to read indirect node [initialize_inode_bitmap]");
                                free(block_buffer);
                                return -1;
                            }

                            for (l = 0; l < NO_OF_DIRECT_INDEXES; l++) {
                                data_block_index = temp_indirect_node.pointers[l];
                                if (data_block_index != 0) {
                                    data_blocks_bitmap[data_block_index - data_block_index_no] = 1;
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
