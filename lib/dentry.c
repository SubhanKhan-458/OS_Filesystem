/**
 * @file dentry.c
 * @author Shaheer Ahmed (k190233@nu.edu.pk) & Noman Vadsariya & Subhan Khan
 * @brief Provides methods for managing directory entry in our FS
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/globals.h"

// TODO: Use hash tables for inode storage/access

/**
 * @brief Initializes the dentry blocks in the block device
 * 
 * @param fd File descriptor
 * @return (int) -1 for error, 1 for ok 
 */
int initialize_dentry_blocks(int * fd) {
    if (fd == NULL || *fd < 0) {
        if (DEBUG == 1) pprintf("Invalid parameters provided [initialize_dentry_blocks]");
        return -1;
    }

    dentry temp;
    temp.inode_index = 0; // 0 indicates empty, subtract 1 from pointer to get the actual pointer

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        if (DEBUG == 1) pprintf("Unable to allocate memory [initialize_dentry_blocks]");
        perror("malloc");
        return -1;
    }

    int i;

    // ensures that any remaining bytes in a block are always null-terminators
    memset(block_buffer, '\0', BLOCK_SIZE);

    // create a block filled with empty dentry
    for (i = 0; i < NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY); i++) {
        memcpy(block_buffer + (i * SIZEOF_DENTRY), &temp, SIZEOF_DENTRY);
    }

    // write dentry blocks starting from the first dentry block index till inode blocks index
    // (it is the same as total no of dentry blocks)
    for (i = DENTRY_BLOCKS_INDEX_NO; i < INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY); i++) {
        if (write_block(fd, (void *) block_buffer, i) == -1) {
            if (DEBUG == 1) pprintf("Unable to write block [initialize_dentry_blocks]");
            free(block_buffer);
            return -1;
        }
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Saves directory entry to block device
 * 
 * @param fd File descriptor
 * @param buffer Directory entry data to write
 * @param dentry_index The target index to write at
 * @return (int) -1 for error, 1 for ok
 */
// try to place dentry early on, i.e. from index 0...
int write_dentry(int * fd, dentry * buffer, int dentry_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        if (DEBUG == 1) pprintf("Invalid parameters provided [write_dentry]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_DENTRY - 1) is equal to the TOTAL_NO_OF_DENTRY,
    // hence when >= is used, not just >
    if (dentry_index < 0 || dentry_index >= TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY)) {
        if (DEBUG == 1) pprintf("Invalid dentry index provided [write_dentry]");
        return -1;
    }

    int block_no = (int) (dentry_index / NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY));
    int dentry_blocks_index = DENTRY_BLOCKS_INDEX_NO;
    if ((block_no + dentry_blocks_index) < 0 || (block_no + dentry_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        if (DEBUG == 1) pprintf("Invalid block no calculated [write_dentry]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        if (DEBUG == 1) pprintf("Unable to allocate memory [write_dentry]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, dentry_blocks_index + block_no) == -1) {
        if (DEBUG == 1) pprintf("Unable to read block [write_dentry]");
        free(block_buffer);
        return -1;
    }

    int offset = (dentry_index - (block_no * NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY))) * SIZEOF_DENTRY;
    memcpy(block_buffer + offset, buffer, SIZEOF_DENTRY);

    if (write_block(fd, (void *) block_buffer, dentry_blocks_index + block_no) == -1) {
        if (DEBUG == 1) pprintf("Unable to write block [write_dentry]");
        free(block_buffer);
        return -1;
    }

    free(block_buffer);
    return 1;
}

/**
 * @brief Saves the targetted dentry's data to the dentry buffer
 * 
 * @param fd File descriptor
 * @param buffer The dentry buffer, where we would save the data
 * @param dentry_index The target dentry's index
 * @return (int) -1 for error, 1 for ok 
 */
int read_dentry(int * fd, dentry * buffer, int dentry_index) {
    if (fd == NULL || *fd < 0 || buffer == NULL) {
        if (DEBUG == 1) pprintf("Invalid parameters provided [read_dentry]");
        return -1;
    }

    // 0 till (TOTAL_NO_OF_INODES - 1) is equal to the TOTAL_NO_OF_INODES,
    // hence when >= is used, not just >
    if (dentry_index < 0 || dentry_index >= TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY)) {
        if (DEBUG == 1) pprintf("Invalid dentry index provided [read_dentry]");
        return -1;
    }

    int block_no = ((int) (dentry_index / NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY)));
    int dentry_blocks_index = DENTRY_BLOCKS_INDEX_NO;
    if ((block_no + dentry_blocks_index) < 0 || (block_no + dentry_blocks_index) > TOTAL_NO_OF_BLOCKS) {
        if (DEBUG == 1) pprintf("Invalid block no calculated [read_dentry]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        if (DEBUG == 1) pprintf("Unable to allocate memory [read_dentry]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, dentry_blocks_index + block_no) == -1) {
        if (DEBUG == 1) pprintf("Unable to read block [read_dentry]");
        free(block_buffer);
        return -1;
    }

    int offset = (dentry_index - (block_no * NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY))) * SIZEOF_DENTRY;
    memcpy(buffer, block_buffer + offset, SIZEOF_DENTRY);

    free(block_buffer);
    return 1;
}

/**
 * @brief Cleans a targetted dentry, resetting all attributes to the "empty state"
 * 
 * "empty state" is the initial state, where the inode_index = 0
 * 
 * @param fd File descriptor
 * @param inode_index The targetted dentry's index
 * @return (int) -1 for error, 1 for ok 
 */
int clean_dentry(int * fd, int dentry_index) {
    dentry temp = {
        .inode_index = 0, // 0 indicates empty, subtract 1 from pointer to get the actual pointer
    };

    if (write_dentry(fd, &temp, dentry_index) == -1) {
        if (DEBUG == 1) pprintf("Unable to clean dentry [clean_dentry]");
        return -1;
    }

    return 1;
}

int djb2_hash(char * string) {
    unsigned long hash = 5381;
    int c;

    while ((c = *string++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return ((int) (hash % (TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY))));
}

int dentry_lookup(int * fd, char * node_name, int nth_occurence) {
    if (fd == NULL || fd < 0 || node_name == NULL) {
        if (DEBUG == 1) pprintf("Invalid parameters provided [dentry_lookup]");
        return -1;
    }
    
    dentry temp;
    int dentry_index = djb2_hash(node_name), i = 0;
    
    while (i < nth_occurence) {
        dentry_index++;
        i++;
    }

    if (read_dentry(fd, &temp, (dentry_index % TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY))) == -1) {
        if (DEBUG == 1) pprintf("Unable to read dentry [dentry_lookup]");
        return -1;
    }

    return temp.inode_index;
}

int dentry_lookup_using_inode_index(int * fd, int inode_index) {
    if (fd == NULL || *fd < 0) {
        if (DEBUG == 1) pprintf("Invalid parameters provided [dentry_lookup_using_inode_index]");
        return -1;
    }

    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        if (DEBUG == 1) pprintf("Invalid inode index provided [dentry_lookup_using_inode_index]");
        return -1;
    }

    int i, max_dentry = TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY);
    dentry dentry_buff;

    for (i = 0; i < max_dentry; i++) {
        if (read_dentry(fd, &dentry_buff, i) == -1) {
            continue;
        }

        if ((dentry_buff.inode_index - 1) != inode_index) {
            continue;
        }

        return i;
    }

    return -1;
}

int dentry_lookup_with_index(int * fd, char * nod_name, int inode_index) {
    if (fd == NULL || fd < 0 || nod_name == NULL) {
        if (DEBUG == 1) pprintf("Invalid parameters provided [dentry_lookup]");
        return -1;
    }

    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        if (DEBUG == 1) pprintf("Invalid inode index provided [dentry_lookup]");
        return -1;
    }

    dentry temp;
    int dentry_index = djb2_hash(nod_name);
    int tries = 0;

    do {
        if (read_dentry(fd, &temp, dentry_index) == -1) {
            if (DEBUG == 1) pprintf("Unable to read dentry [dentry_lookup]");
            return -1;
        }

        tries++;
        dentry_index++; // move the dentry_index forward, look ahead in case of collision
    } while ((tries < TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY)) && (strcmp(temp.filename, nod_name) != 0 && inode_index != (temp.inode_index)));

    return temp.inode_index;
}

int add_dentry(int * fd, char * inode_name, int inode_name_len, int inode_index) {
    if (fd == NULL || *fd < 0 || inode_name == NULL) {
        if (DEBUG == 1) pprintf("Invalid parameters provided [add_dentry]");
        return -1;
    }

    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        if (DEBUG == 1) pprintf("Invalid inode index provided [add_dentry]");
        return -1;
    }

    char * inode_name_buff = (char *) malloc(sizeof(char) * inode_name_len + 1);
    if (inode_name_buff == NULL) {
        // unable to alloc mem
        return -1;
    }

    // +1 to add space for null-terminator and memset to fill all bytes with null-terminator
    memset(inode_name_buff, '\0', sizeof(char) * inode_name_len + 1);
    memcpy(inode_name_buff, inode_name, inode_name_len);

    int dentry_index = djb2_hash(inode_name);
    int hash_result = dentry_index;
    dentry dentry_buff;
    int dentry_inode_index;

    do {
        if (read_dentry(fd, &dentry_buff, dentry_index) == -1) {
            if (DEBUG == 1) pprintf("Unable to read dentry [add_dentry]");
            return -1;
        }

        dentry_inode_index = (dentry_buff.inode_index - 1);
        if (dentry_inode_index == -1) {
            memcpy(dentry_buff.filename, inode_name_buff, sizeof(char) * inode_name_len + 1); //, (sizeof(char) * strlen(name)));
            dentry_buff.inode_index = (inode_index + 1);
            
            if (write_dentry(fd, &dentry_buff, dentry_index) == -1) {
                if (DEBUG == 1) pprintf("Unable to write dentry [add_dentry]");
                return -1;
            }

            return 1;
        }

        // if dentry_index is already filled
        dentry_index++;
        
        // run the loop till we do a complete 360 (reach the starting point again)
    } while (((dentry_index % TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY)) != hash_result));

    return -1;
}

/**
 * @brief Dumps a dump_dentry's data to stdout
 * 
 * @param buffer The dump_dentry whose data we want to dump
 */
void dump_dentry(dentry * buffer) {
    if (buffer == NULL) {
        printf("Invalid parameters provided [dump_dentry]");
        return;
    }

    printf("--------------------------------------------\n");
    printf("DENTRY\n");
    printf("--------------------------------------------\n");
    printf("filename = %s,\n", buffer->filename);
    printf("inode index = %d\n", buffer->inode_index);

    printf("--------------------------------------------\n");

    return;
}
