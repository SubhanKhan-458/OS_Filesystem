#include "../include/globals.h"

// We know the size of the data
// We need to divide data into chunks of BLOCK_SIZE
// Each chunk is written depending on its size
// If the size is less than the BLOCK_SIZE then we look for used data blocks that can accomodate the buffer data
// If we couldn't find space in direct data blocks, we then look for it in indirect data blocks
// If we couldnt find it in indirect data blocks, then we utilize completely empty direct blocks
// If there aren't any empty direct blocks, we access the indirect node's blocks and look for empty ones

// remember to update inodes and indirect nodes after writes

// we can use the data block bitmap to denote full and half-full blocks
// such as 1 for full, 2 for half-full, 0 for empty

// remember when deleting data, to replace blocks with '\0'

int write_data(int * fd, char * buffer, int buffer_size, int inode_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL || buffer_size < 0) {
        pprintf("Invalid parameters provided [write_data]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INODES - 1) is equal to the TOTAL_NO_OF_INODES,
    // hence when >= is used, not just >
    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid inode index provided [write_data]");
        return -1;
    }

    // char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    // if (block_buffer == NULL) {
    //     pprintf("Unable to allocate memory [write_data]");
    //     perror("malloc");
    //     return -1;
    // }

    // bytes left to write
    int bytes_left = buffer_size;
    inode temp_inode;

    read_inode(fd, &temp_inode, inode_index);

    if (bytes_left < BLOCK_SIZE) {
        if (fill_used_blocks(fd, buffer, bytes_left, &temp_inode)) {
            return 1;
        }

        if (fill_free_blocks(fd, buffer, bytes_left, &temp_inode)) {
            return 1;
        }

        pprintf("Unable to write to inode; data blocks full [write_data]");
        return -1;
    } else {
        while (bytes_left >= BLOCK_SIZE) {
            fill_free_blocks(fd, buffer, buffer_size, &temp_inode);
        }

        if (bytes_left > 0 && bytes_left < BLOCK_SIZE) {
            if (fill_used_blocks(fd, buffer, buffer_size, &temp_inode)) {
                return 1;
            }

            if (fill_free_blocks(fd, buffer, buffer_size, &temp_inode)) {
                return 1;
            }

            pprintf("Unable to write to inode; data blocks full [write_data]");
            return -1;
        }
    }

    if (bytes_left == 0) {
        return 1;
    }

    pprintf("Unable to write to inode; data blocks full [write_data]");
    return -1;
}

int fill_used_blocks(int * fd, char * buffer, int buffer_size, inode * inode_buffer) {
    if (fd == NULL || *fd < 0 || buffer == NULL || buffer_size < 0 || inode_buffer == NULL) {
        pprintf("Invalid parameters provided [fill_used_blocks]");
        return -1;
    }

    if (buffer_size > BLOCK_SIZE) {
        pprintf("Provided buffer must be less than BLOCK_SIZE [fill_used_blocks]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [fill_used_blocks]");
        perror("malloc");
        return -1;
    }

    if (write_remaining_buffer_to_block(fd, block_buffer, buffer, buffer_size, &(inode_buffer->pointers)) == 1) {
        free(block_buffer);
        return 1;
    }

    indirect_node temp_indirect_node;
    int i;
    for (i = NO_OF_DIRECT_INDEXES; i < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); i++) {
        if (inode_buffer->pointers[i] == 0) {
            continue;
        }

        read_indirect_node(fd, &temp_indirect_node, inode_buffer->pointers[i]);
        if (write_remaining_buffer_to_block(fd, block_buffer, buffer, buffer_size, &(temp_indirect_node.pointers)) == 1) {
            free(block_buffer);
            return 1;
        }
    }

    pprintf("Unable to write block [fill_used_blocks]");
    free(block_buffer);
    return -1;
}

// this can write to direct and single indirect
int write_remaining_buffer_to_block(int * fd, char * block_buffer, char * buffer, int buffer_size, int * pointers) {
    if (fd == NULL || *fd < 0 || block_buffer == NULL || buffer == NULL || buffer_size < 0 || pointers == NULL) {
        pprintf("Invalid parameters provided [write_remaining_buffer_to_block]");
        return -1;
    }

    if (buffer_size > BLOCK_SIZE) {
        pprintf("Provided buffer must be less than BLOCK_SIZE [write_remaining_buffer_to_block]");
        return -1;
    }

    int i, data_block_index, bytes_read;

    for (i = 0; i < (NO_OF_DIRECT_INDEXES); i++) {
        // 0 indicates empty, must start from DATA_BLOCK_INDEX_NO
        data_block_index = pointers[i];
        // -1, 0, 1 signify invalid, empty and full respectively
        if (get_data_block_bitmap_value(data_block_index) <= 1) {
            continue;
        }

        if (read_block(fd, (void *) block_buffer, data_block_index) == -1) {
            pprintf("Unable to read block [write_remaining_buffer_to_block]");
            // free(block_buffer);
            return -1;
        }

        // read till null-terminator is found, or till BLOCK_SIZE is reached
        for (bytes_read = 0; (block_buffer[bytes_read] != '\0') && (bytes_read < BLOCK_SIZE); bytes_read++);

        // write to it
        // NOTE: when we write a complete block, we'll be overwriting the null-terminator at the end as well
        if ((BLOCK_SIZE - bytes_read) >= buffer_size) {
            // should overwrite the pre-existing null-terminator
            // the next null-terminator would automatically exist, since
            // null-terminator is what data blocks consists of by default
            memcpy(block_buffer + bytes_read, buffer, buffer_size);
            write_block(fd, (void *) block_buffer, data_block_index);
            
            if ((BLOCK_SIZE - bytes_read) == buffer_size) {
                set_data_block_bitmap_value(data_block_index, 1); // set data block to full
            }

            return 1;
        }
    }

    pprintf("Unable to write block [write_remaining_buffer_to_block]");
    return -1;
}

int fill_free_blocks(int * fd, char * buffer, int buffer_size, inode * inode_buffer) {
    if (fd == NULL || *fd < 0 || buffer == NULL || buffer_size < 0 || inode_buffer == NULL) {
        pprintf("Invalid parameters provided [fill_free_blocks]");
        return -1;
    }

    if (buffer_size != BLOCK_SIZE) {
        pprintf("Provided buffer must be equal to BLOCK_SIZE [fill_free_blocks]");
        return -1;
    }

    if (write_full_buffer_to_block(fd, buffer, buffer_size, &(inode_buffer->pointers), 0) == 1) {
        return 1;
    }

    indirect_node temp_indirect_node;
    int i;
    for (i = NO_OF_DIRECT_INDEXES; i < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); i++) {
        // Assuming that we'll be moving to data to the earliest empty space avaialable
        // i.e. data from indirect nodes would be moved to direct blocks if space becomes available
        // upon deletion, as a result successive pointers would be empty if previous one is
        if (inode_buffer->pointers[i] == 0) {
            
        }

        read_indirect_node(fd, &temp_indirect_node, inode_buffer->pointers[i]);
        if (write_full_buffer_to_block(fd, buffer, buffer_size, &(temp_indirect_node.pointers), 1) == 1) {
            return 1;
        }
    }

    pprintf("Unable to write block [fill_free_blocks]");
    return -1;
}

int write_full_buffer_to_block(int * fd, char * buffer, int buffer_size, int * pointers, int is_indirect_node) {
    if (fd == NULL || *fd < 0 || buffer == NULL || buffer_size < 0 || pointers == NULL) {
        pprintf("Invalid parameters provided [write_full_buffer_to_block]");
        return -1;
    }

    if (buffer_size != BLOCK_SIZE) {
        pprintf("Provided buffer must be equal to BLOCK_SIZE [write_full_buffer_to_block]");
        return -1;
    }

    int i, data_block_index;

    for (i = 0; i < (NO_OF_DIRECT_INDEXES); i++) {
        // 0 indicates empty, must start from DATA_BLOCK_INDEX_NO
        data_block_index = pointers[i];
        // -1, 0, 1 signify invalid, empty and full respectively
        if (get_data_block_bitmap_value(data_block_index) != 0) {
            continue;
        }

        data_block_index = get_free_data_block_index();
        if (data_block_index < 0) {
            pprintf("No free data block is avaialble [write_full_buffer_to_block]");
            return -1;
        }

        // buffer here should be of BLOCK_SIZE
        write_block(fd, (void *) buffer, data_block_index);

        return 1;
    }

    return -1;
}
