/**
 * @file inodes.c
 * @author Shaheer Ahmed (k190233@nu.edu.pk)
 * @brief Provides methods for managing inodes for our FS
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

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
    if (fd == NULL || *fd < 0) {
        pprintf("Invalid parameters provided [clean_inode]");
        return -1;
    }

    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid inode index provided [clean_inode]");
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

    if (write_inode(fd, &temp, inode_index) == -1) {
        pprintf("Unable to clean inode [clean_inode]");
        return -1;
    }

    // cleaned inode, mark it as empty now
    set_inode_bitmap_value(inode_index, 0);

    return 1;
}

int add_inode(int * fd, inode * buffer) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        pprintf("Invalid parameters provided [add_inode]");
        return -1;
    }

    int inode_index = get_free_inode_index();
    if (inode_index == -1) {
        return -1;
    }

    if (write_inode(fd, buffer, inode_index) == -1) {
        return -1;
    }

    // set inode bitmap value
    set_inode_bitmap_value(inode_index, 1);

    return inode_index;
}

int empty_inode(int * fd, inode * inode_buff, int inode_index) {
    if (fd == NULL || *fd < 0 || inode_buff == NULL) {
        pprintf("Invalid parameters provided [empty_inode]");
        return -1;
    }

    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid inode index provided [empty_inode]");
        return -1;
    }

    // clean data blocks
    if (empty_data_blocks(fd, inode_buff->pointers) == -1) {
        return -1;
    }

    int i, indirect_node_index;
    indirect_node indirect_node_buff;

    for (i = NO_OF_DIRECT_INDEXES; i < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); i++) {
        indirect_node_index = (inode_buff->pointers[i] - 1);
        if (indirect_node_index == -1) {
            continue;
        }

        if (read_indirect_node(fd, &indirect_node_buff, indirect_node_index) == -1) {
            return -1;
        }

        if (empty_data_blocks(fd, indirect_node_buff.pointers) == -1) {
            return -1;
        }

        set_indirect_node_bitmap_value(indirect_node_index, 0);
    }

    // clean inode
    if (clean_inode(fd, inode_index) == -1) {
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
