/**
 * @file data_blocks.c
 * @author Shaheer Ahmed (k190233@nu.edu.pk)
 * @brief Provides methods for managing data blocks in our FS
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/globals.h"

// remember when deleting data, to replace blocks with '\0'

/**
 * @brief Writes data of arbitrary length to the targetted inode's data blocks
 * 
 * @param fd File descriptor
 * @param buffer The data buffer
 * @param buffer_size The data buffer's size (in bytes)
 * @param inode_index The targetted inode's index
 * @return (int) -1 for error, 1 for ok 
 */
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

    // bytes left to write
    int bytes_left = buffer_size;
    inode temp_inode;

    if (read_inode(fd, &temp_inode, inode_index) == -1) {
        pprintf("Unable to read inode [write_data]");
        return -1;
    }

    if (temp_inode.type == IS_EMPTY_INODE) {
        pprintf("Invalid inode index provided [write_data]");
        return -1;
    }

    if (bytes_left < BLOCK_SIZE) {
        if (fill_used_blocks(fd, buffer, bytes_left, &temp_inode) == 1) {
            return 1;
        }

        if (fill_free_blocks(fd, buffer, bytes_left, &temp_inode, inode_index) == 1) {
            return 1;
        }

        pprintf("Unable to write to inode; data blocks full [write_data]");
        return -1;
    } else {
        char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
        if (block_buffer == NULL) {
            pprintf("Unable to allocate memory [write_data]");
            perror("malloc");
            return -1;
        }

        int blocks_written = 0, i, j, data_block_index, partitioned_byte_count, partition_handled = 0;

        while (bytes_left >= BLOCK_SIZE) {
            // calculate used block's remaining bytes
            // since bytes_left would be greater than or equal to BLOCK_SIZE
            // we would firstly partitioned the buffer by finding a used block 
            // and calculating the remaining byte count, then writing that many 
            // bytes to fill up that block
            if (partition_handled == 0) {
                if (fill_used_block_with_partition(fd, block_buffer, buffer, &temp_inode, &bytes_left) == -1) {
                    pprintf("Unable to partition and write [write_data] {this is not necessarily an error! (NOT ABORTING!)}");
                }

                partition_handled = 1;
                continue;
            }

            memcpy(block_buffer, buffer + (blocks_written * BLOCK_SIZE * sizeof(char)), sizeof(char) * BLOCK_SIZE);
            if (fill_free_blocks(fd, block_buffer, (sizeof(char) * BLOCK_SIZE), &temp_inode, inode_index) == -1) {
                pprintf("Unable to fill free blocks [write_data]");
                free(block_buffer);
                return -1;
            }

            bytes_left -= (sizeof(char) * BLOCK_SIZE);
            blocks_written++;
        }

        if (bytes_left > 0 && bytes_left < BLOCK_SIZE) {
            memcpy(block_buffer, buffer + (blocks_written * BLOCK_SIZE * sizeof(char)), bytes_left);
            if (fill_used_blocks(fd, block_buffer, bytes_left, &temp_inode) == 1) {
                free(block_buffer);
                return 1;
            }

            if (fill_free_blocks(fd, block_buffer, bytes_left, &temp_inode, inode_index) == 1) {
                free(block_buffer);
                return 1;
            }

            pprintf("Unable to write to inode; data blocks full [write_data]");
            free(block_buffer);
            return -1;
        }

        free(block_buffer);
    }

    if (bytes_left == 0) {
        // update inode's size
        inode inode_buff;
        if (read_inode(fd, &inode_buff, inode_index) == -1) {
            pprintf("Unable to read inode [write_data]");
            return -1;
        }

        inode_buff.size += buffer_size;
        if (write_inode(fd, &inode_buff, inode_index) == -1) {
            pprintf("Unable to write inode [write_data]");
            return -1;
        }

        return 1;
    }

    pprintf("Unable to write to inode; data blocks full [write_data]");
    return -1;
}

/**
 * @brief Handles writes to blocks that were somewhat filled
 * 
 * We do this to completely utilize all available space, without leaving any holes
 * 
 * @param fd File descriptor
 * @param buffer The data buffer
 * @param buffer_size The data buffer's size
 * @param inode_buffer The targetted inode
 * @return (int) -1 for error, 1 for ok 
 */
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

    if (write_remaining_buffer_to_block(fd, block_buffer, buffer, buffer_size, inode_buffer->pointers) == 1) {
        free(block_buffer);
        return 1;
    }

    indirect_node temp_indirect_node;
    int i, indirect_node_index;
    for (i = NO_OF_DIRECT_INDEXES; i < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); i++) {
        indirect_node_index = (inode_buffer->pointers[i] - 1);
        if (indirect_node_index == -1) {
            continue;
        }

        if (read_indirect_node(fd, &temp_indirect_node, indirect_node_index) == -1) {
            pprintf("Unable to read indirect node [fill_used_blocks]");
            return -1;
        }

        if (write_remaining_buffer_to_block(fd, block_buffer, buffer, buffer_size, temp_indirect_node.pointers) == 1) {
            free(block_buffer);
            return 1;
        }
    }

    pprintf("Unable to write block [fill_used_blocks] {this might be due to not having any half-full blocks! (not necessary an error)}");
    free(block_buffer);
    return -1;
}

/**
 * @brief Writes buffers of less than BLOCK_SIZE bytes to the data blocks
 * 
 * @param fd File descriptor
 * @param block_buffer The buffer to load our used block into
 * @param buffer The data buffer
 * @param buffer_size The data buffer's size
 * @param pointers An array of pointers to data blocks
 * @return (int) -1 for error, 1 for ok 
 */
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

    // will unnecessarily print a misleading message in case of not having any half-full blocks
    // pprintf("Unable to write block [write_remaining_buffer_to_block]");
    return -1;
}

/**
 * @brief Writes buffers of BLOCK_SIZE bytes to completely free data blocks
 * 
 * @param fd File descriptor
 * @param buffer The data buffer
 * @param buffer_size The data buffer's size
 * @param inode_buffer The targetted inode
 * @param inode_index The index to the targetted inode
 * @return (int) -1 for error, 1 for ok 
 */
int fill_free_blocks(int * fd, char * buffer, int buffer_size, inode * inode_buffer, int inode_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL || buffer_size < 0 || inode_buffer == NULL) {
        pprintf("Invalid parameters provided [fill_free_blocks]");
        return -1;
    }

    // if (buffer_size != BLOCK_SIZE) {
    //     pprintf("Provided buffer must be equal to BLOCK_SIZE [fill_free_blocks]");
    //     return -1;
    // }

    int data_block_index, pointer_index;

    if (( data_block_index = write_full_buffer_to_block(fd, buffer, buffer_size, inode_buffer->pointers, &pointer_index)) != -1) {
        inode_buffer->pointers[pointer_index] = data_block_index;
        if (write_inode(fd, inode_buffer, inode_index) == -1) {
            pprintf("Unable to write to inode [fill_free_blocks]");
            return -1;
        }

        return 1;
    }

    indirect_node temp_indirect_node;
    int i, indirect_node_index;
    for (i = NO_OF_DIRECT_INDEXES; i < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); i++) {
        // Assuming that we'll be moving to data to the earliest empty space avaialable
        // i.e. data from indirect nodes would be moved to direct blocks if space becomes available
        // upon deletion, as a result successive pointers would be empty if previous one is
        indirect_node_index = (inode_buffer->pointers[i] - 1);
        if (indirect_node_index == -1) {
            indirect_node_index = get_free_indirect_node_index();
            if (indirect_node_index < 0) {
                pprintf("No free indirect node is available [fill_free_blocks]");
                return -1;
            }

            // initializes an indirect node and leaves it at an empty state
            if (clean_indirect_node(fd, indirect_node_index) == -1) {
                pprintf("Unable to initialize inode [fill_free_blocks]");
                return -1;
            }

            set_indirect_node_bitmap_value(indirect_node_index, 1);

            // +1 because 0 signifies empty
            inode_buffer->pointers[i] = (indirect_node_index + 1);
            
            if (write_inode(fd, inode_buffer, inode_index) == -1) {
                pprintf("Unable to write to inode [fill_free_blocks]");
                return -1;
            }
        }

        if (read_indirect_node(fd, &temp_indirect_node, indirect_node_index) == -1) {
            pprintf("Unable to read indirect node [fill_free_blocks]");
            return -1;
        }

        if (( data_block_index = write_full_buffer_to_block(fd, buffer, buffer_size, temp_indirect_node.pointers, &pointer_index)) != -1) {
            temp_indirect_node.pointers[pointer_index] = data_block_index;
            if (write_indirect_node(fd, &temp_indirect_node, indirect_node_index) == -1) {
                pprintf("Unable to write indirect node [fill_free_blocks]");
                return -1;
            }

            return 1;
        }
    }

    pprintf("Unable to write block [fill_free_blocks]");
    return -1;
}

/**
 * @brief Writes the entirety of a buffer to completely free block
 * 
 * @param fd File descriptor
 * @param buffer The data buffer
 * @param buffer_size The data buffer's size
 * @param pointers An array of pointers to data blocks
 * @param pointer_index The index to the pointer we just wrote to
 * @return (int) -1 for error, 1 for ok 
 */
int write_full_buffer_to_block(int * fd, char * buffer, int buffer_size, int * pointers, int * pointer_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL || buffer_size < 0 || pointers == NULL) {
        pprintf("Invalid parameters provided [write_full_buffer_to_block]");
        return -1;
    }

    // if (buffer_size != BLOCK_SIZE) {
    //     pprintf("Provided buffer must be equal to BLOCK_SIZE [write_full_buffer_to_block]");
    //     return -1;
    // }

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

        // we'll create an aux buffer filled with null-terminators to mitigate any damage to our 
        // assumption based code
        // TODO: Refactor later to pass as an argument, since it would be allocated/deallocated rapidly
        char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
        if (block_buffer == NULL) {
            pprintf("Unable to allocate memory [write_full_buffer_to_block]");
            perror("malloc");
            return -1;
        }

        memset(block_buffer, '\0', (sizeof(char) * BLOCK_SIZE));
        memcpy(block_buffer, buffer, buffer_size);

        // buffer here should be of BLOCK_SIZE
        write_block(fd, (void *) block_buffer, data_block_index);
        set_data_block_bitmap_value(data_block_index, (buffer_size == BLOCK_SIZE ? 1 : 2));

        *pointer_index = i;
        free(block_buffer);
        return data_block_index;
    }

    return -1;
}

/**
 * @brief Writes to already used blocks by partioning buffer to fill the used block completely
 * 
 * @param fd File descriptor
 * @param block_buffer An auxiliary buffer to support approriate writes to our blocks
 * @param buffer The data buffer
 * @param inode_buffer The targetted inode
 * @param bytes_left Bytes left to be written
 * @return (int) -1 for error, 1 for ok 
 */
int fill_used_block_with_partition(int * fd, char * block_buffer, char * buffer, inode * inode_buffer, int * bytes_left) {
    if (fd == NULL || *fd < 0 || buffer == NULL || block_buffer == NULL || inode_buffer == NULL) {
        pprintf("Invalid parameters provided [fill_used_block_with_partition]");
        return -1;
    }

    // let the control flow fall down to indirect nodes
    if (partition_and_write(fd, block_buffer, buffer, inode_buffer->pointers, inode_buffer, bytes_left) == 1) {
        return 1;
    }

    pprintf("Unable to partition and write to data block [fill_used_block_with_partition] {this isn't necessarily an error!}");
    
    int i, indirect_node_index;
    indirect_node temp_indirect_node;

    for (i = NO_OF_DIRECT_INDEXES; i < (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES); i++) {
        // -1 to get actual indirect node index
        indirect_node_index = (inode_buffer->pointers[i] - 1);

        if (indirect_node_index == -1) {
            pprintf("Invalid indirect node index [fill_used_block_with_partition]");
            continue;
        }

        if (read_indirect_node(fd, &temp_indirect_node, indirect_node_index) == -1) {
            pprintf("Unable to read indirect node [fill_used_block_with_partition]");
            return -1;
        }

        if (partition_and_write(fd, block_buffer, buffer, temp_indirect_node.pointers, inode_buffer, bytes_left) == -1) {
            pprintf("Unable to partition and write to indirect node [fill_used_block_with_partition] {this isn't necessarily an error!}");
            // return -1;
        }
    }

    // might've been managed, might've not been managed, meh
    return 1;
}

/**
 * @brief Partitions data and writes it to the available data blocks that can hold it
 * 
 * @param fd File descriptor
 * @param block_buffer An auxiliary block to support approriate writes to our blocks
 * @param buffer The data buffer
 * @param pointers An array of pointers to data blocks
 * @param inode_buffer The targetted inode
 * @param bytes_left Bytes left to be written
 * @return (int) -1 for error, 1 for ok 
 */
int partition_and_write(int * fd, char * block_buffer, char * buffer, int * pointers, inode * inode_buffer, int * bytes_left) {
    if (fd == NULL || *fd < 0 || buffer == NULL || block_buffer == NULL || inode_buffer == NULL) {
        pprintf("Invalid parameters provided [partition_and_write]");
        return -1;
    }

    int i, data_block_index, partitioned_byte_count;
    
    for (i = 0; i < (NO_OF_DIRECT_INDEXES); i++) {
        data_block_index = pointers[i];
        if (data_block_index == 0) {
            continue;
        }

        if (get_data_block_bitmap_value(data_block_index) == 2) {
            partitioned_byte_count = get_remaining_byte_count(fd, data_block_index);
            // not adding blocks_written because I expect for this to run on the first go, 
            // and only once
            memcpy(block_buffer, buffer, partitioned_byte_count);
            if (fill_used_blocks(fd, block_buffer, partitioned_byte_count, inode_buffer) == -1) {
                pprintf("Unable to fill used blocks for partitioned data [fill_used_block_with_partition]");
                return -1;
            }
        
            *bytes_left -= (sizeof(char) * partitioned_byte_count);
            return 1;
        }
    }

    // might not have any used blocks
    return -1;
}

/**
 * @brief Gets the bytes left out of a particular data block
 * 
 * Returns the size of the hole that we can write to
 * 
 * @param fd File descriptor
 * @param data_block_index The targetted data block's index
 * @return (int) -1 for error, 1 for ok 
 */
int get_remaining_byte_count(int * fd, int data_block_index) {
    if (fd == NULL || *fd < 0 ) {
        pprintf("Invalid parameters provided [get_remaining_byte_count]");
        return -1;
    }

    if (data_block_index == 0) {
        return 0;
    }

    int i = (data_block_index - DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE));
    if (i < 0 || i > TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE)) {
        pprintf("Invalid data block index provided [get_remaining_byte_count]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [get_remaining_byte_count]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, data_block_index) == -1) {
        pprintf("Unable to read block [get_remaining_byte_count]");
        free(block_buffer);
        return -1;
    }

    int bytes_read = 0;

    for (bytes_read = 0; (block_buffer[bytes_read] != '\0') && (bytes_read < BLOCK_SIZE); bytes_read++);

    // if last byte is null-terminator then the block is regarded as full
    if (block_buffer[bytes_read] == '\0' && (bytes_read + 1) == BLOCK_SIZE) {
        free(block_buffer);
        return BLOCK_SIZE;
    }

    free(block_buffer);
    return BLOCK_SIZE - bytes_read;
}

// inode_pointer points to whether this is a direct block (< NO_OF_DIRECT_INDEXES) or
// an indirect node (>= NO_OF_DIRECT_INDEXES)
// data_pointer is from 0 till (NO_OF_DIRECT_INDEXES - 1)
int read_data_by_block(int * fd, char * block_buffer, inode * inode_buffer, int pointer) {
    if (fd == NULL || *fd < 0 || block_buffer == NULL || inode_buffer == NULL) {
        pprintf("Invalid parameters provided [read_data_by_block]");
        return -1;
    }

    if (pointer < 0 || pointer >= (NO_OF_DIRECT_INDEXES + (NO_OF_DIRECT_INDEXES * NO_OF_INDIRECT_INDEXES))) {
        pprintf("Invalid pointer index provided (for data blocks) [read_data_by_block]");
        return -1;
    }

    int data_block_index = 0;

    if (pointer >= NO_OF_DIRECT_INDEXES) {
        indirect_node indirect_node_buff;
        float calc_buff = pointer - NO_OF_DIRECT_INDEXES;
        int pointer_to_indirect_node = calc_buff / NO_OF_DIRECT_INDEXES;
        int indirect_node_index = inode_buffer->pointers[NO_OF_DIRECT_INDEXES + pointer_to_indirect_node] - 1;

        if (indirect_node_index == -1) {
            pprintf("Invalid indirect node index [read_data_by_block]");
            return -1;
        }

        if (read_indirect_node(fd, &indirect_node_buff, indirect_node_index) == -1) {
            pprintf("Unable to read indirect node [read_data_by_block]");
            return -1;
        }

        data_block_index = indirect_node_buff.pointers[pointer % (NO_OF_DIRECT_INDEXES)];
        if (data_block_index == 0) {
            pprintf("Unable to read block [read_data_by_block]");
            return -1;
        }

        if (read_block(fd, (void *) block_buffer, data_block_index) == -1) {
            pprintf("Unable to read block [read_data_by_block]");
            return -1;
        }

        return 1;
    }

    data_block_index = inode_buffer->pointers[pointer];
    if (data_block_index == 0) {
        pprintf("Unable to read block [read_data_by_block]");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, data_block_index) == -1) {
        pprintf("Unable to read block [read_data_by_block]");
        return -1;
    }

    return 1;
}
