#include "../include/globals.h"

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
    memset(temp.pointers, 0, (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES));

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
        pprintf("Invalid block_no calculated [read_inode]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [read_inode]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY) + block_no) == -1) {
        pprintf("Unable to read block [read_inode]");
        free(block_buffer);
        return -1;
    }

    int offset = (inode_index - (block_no * NO_OF_INODES_PER_BLOCK(SIZEOF_INODE))) * SIZEOF_INODE;
    memcpy(buffer, block_buffer + offset, SIZEOF_INODE);

    free(block_buffer);
    return 1;
}

void dump_inode(inode * buffer) {
    if (buffer == NULL) {
        printf("Invalid parameters provided [dump_inode]");
        return;
    }

    char type_empty_msg[] = "EMPTY";
    char type_dir_msg[] = "DIRECTORY";
    char type_file_msg[] = "FILE";

    printf("INODE\n");
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

    return;
}
