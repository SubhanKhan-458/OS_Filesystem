#include "../include/globals.h"

void dump_to_file(const char *, const char *, char *, int);
int init_dev(const char *, int *);
void close_dev(int *);

int main () {
    int fd;

    printf("TOTAL_NO_OF_BLOCKS = %d\n", TOTAL_NO_OF_BLOCKS);
    printf("TOTAL_NO_OF_INODE_BLOCKS = %d\n", TOTAL_NO_OF_INODE_BLOCKS);
    printf("TOTAL_NO_OF_INDIRECT_NODE_BLOCKS = %d\n", TOTAL_NO_OF_INDIRECT_NODE_BLOCKS(SIZEOF_INODE, SIZEOF_INDIRECT_NODE));
    printf("TOTAL_NO_OF_DENTRY_BLOCKS = %d\n", TOTAL_NO_OF_DENTRY_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("TOTAL_NO_OF_DATA_BLOCKS = %d\n", TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE));
    printf("TOTAL_NO_OF_INODES = %d\n", TOTAL_NO_OF_INODES(SIZEOF_INODE));
    printf("TOTAL_NO_OF_INDIRECT_NODES = %d\n", TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE));
    printf("TOTAL_NO_OF_DENTRY = %d\n", TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("NO_OF_INODES_PER_BLOCK = %d\n", NO_OF_INODES_PER_BLOCK(SIZEOF_INODE));
    printf("NO_OF_INDIRECT_NODES_PER_BLOCK = %d\n", NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE));
    printf("NO_OF_DENTRY_PER_BLOCK = %d\n", NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY));
    printf("SUPER_BLOCK_INDEX_NO = %d\n", SUPER_BLOCK_INDEX_NO);
    printf("DENTRY_BLOCKS_INDEX_NO = %d\n", DENTRY_BLOCKS_INDEX_NO);
    printf("INODE_BLOCKS_INDEX_NO = %d\n", INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("INDIRECT_NODE_BLOCKS_INDEX_NO = %d\n", INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("DATA_BLOCKS_INDEX_NO = %d\n", DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE));


    init_dev("/home/shaheer/OS_Filesystem/temp/foo.img", &fd);

    inode temp_inode;
    if (read_inode(&fd, &temp_inode, 0) != -1) {
        dump_inode(&temp_inode);
    }
    
    indirect_node temp_indirect_node;
    if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
        dump_indirect_node(&temp_indirect_node);
        if (add_pointer_to_indirect_node(&fd, 0, 0, DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) + 1) != -1) {
            if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
                dump_indirect_node(&temp_indirect_node);
            }
        }

        if (add_pointer_to_indirect_node(&fd, 0, 9, DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) + 8) != -1) {
            if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
                dump_indirect_node(&temp_indirect_node);
            }
        }

        if (remove_pointer_from_indirect_node(&fd, 0, 0) != -1) {
            if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
                dump_indirect_node(&temp_indirect_node);
            }
        }
    }

    if (read_indirect_node(&fd, &temp_indirect_node, 1) != -1) {
        dump_indirect_node(&temp_indirect_node);
    }

    if (read_indirect_node(&fd, &temp_indirect_node, 360) != -1) {
        dump_indirect_node(&temp_indirect_node);
    }

    dentry temp_dentry;
    if (read_dentry(&fd, &temp_dentry, 0) != -1) {
        dump_dentry(&temp_dentry);
    }

    if (read_dentry(&fd, &temp_dentry, 226) != -1) {
        dump_dentry(&temp_dentry);
    }

    if (read_dentry(&fd, &temp_dentry, 184) != -1) {
        dump_dentry(&temp_dentry);
    }

    // temp
    // char * bl = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    // read_block(&fd, (void *) bl, 0);
    // dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "w", bl, BLOCK_SIZE);
    // free(bl);

    close_dev(&fd);

    return 0;
}

/**
 * @brief Dumps the contents of a buffer to a specified file
 * 
 * It will stop writing to the file when the target size is reached
 * or when a null-terminator ('\0') is found
 * 
 * @param path Path to our dump file
 * @param modes File modes ("r", "w", "a"...)
 * @param data The data we want to dump
 * @param n Size of data (in bytes)
 */
void dump_to_file(const char * path, const char * modes, char * data, int n) {
    FILE * fp = fopen(path, modes);
    int i;

    if (fp == NULL) {
        pprintf("Unable to open file for dumping... [dump_to_file]");
        return;
    }

    for (i = 0; (i < n) && (data[i] != '\0'); i++) {
        fprintf(fp, "%c ", data[i]);
    }

    fclose(fp);

    return;
}

/**
 * @brief Initializes the block device, from creating the device file to 
 * populating the inodes and writing the superblock
 * 
 * Compares the first 4 bytes of the file with the filesystem's magic number, 
 * if it is the same, it only initializes the bitmaps, else it initializes everything
 * 
 * @param path Path to block device (file)
 * @param fd File descriptor (we write the returned fd to this)
 * @return (int) -1 for error, 1 for ok
 */
int init_dev(const char * path, int * fd) {
    if (path == NULL || fd == NULL) {
        pprintf("Invalid parameters provided [init_dev]");
        return -1;
    }

    *fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (*fd == -1) {
        pprintf("Unable to open file [init_dev]");
        perror("open");
        return -1;
    }

    // read first block
    // if first block has superblock which contains correct magic number
    // then no need to initialize inodes and everything, just the bitmaps
    // else initialize everything from scratch
    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [init_dev]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, 0) == -1) {
        pprintf("Unable to read_block [init_dev]");
        free(block_buffer);
        return -1;
    }

    super_block sb;

    memcpy(&sb.magic_number, block_buffer, sizeof(__uint32_t));
    
    if (sb.magic_number != FS_MAGIC_NUMBER) {
        // initialize inode blocks, data blocks etc.
        // everything gets initialized from scratch
        // first time

        // clean all the blocks first
        if (clean_all_blocks(fd) == -1) {
            pprintf("Unable to clean all blocks [init_dev]");
            free(block_buffer);
            return -1;
        }

        sb.magic_number = (u_int32_t) FS_MAGIC_NUMBER;
        sb.total_blocks = (u_int32_t) TOTAL_NO_OF_BLOCKS;
        sb.total_inode_blocks = (u_int32_t) TOTAL_NO_OF_INODE_BLOCKS;
        sb.total_no_of_inodes = (u_int32_t) TOTAL_NO_OF_INODES(SIZEOF_INODE);

        memcpy(block_buffer, &sb, sizeof(super_block));

        // write super block
        if (write_block(fd, (void *) block_buffer, SUPER_BLOCK_INDEX_NO) == -1) {
            pprintf("Unable to write super block [init_dev]");
            free(block_buffer);
            return -1;
        }

        // write dentry blocks
        if (initialize_dentry_blocks(fd) == -1) {
            pprintf("Unable to write dentry blocks [init_dev]");
            free(block_buffer);
            return -1;
        }

        // root dentry, represents the "/" directory and root
        dentry sb_dentry = {
            .filename = "/",
            .inode_index = 0,
        };

        if (write_dentry(fd, &sb_dentry, 0) == -1) {
            pprintf("Unable to write dentry [init_dev]");
            free(block_buffer);
            return -1;
        }

        // write inode blocks
        if (initialize_inode_blocks(fd) == -1) {
            pprintf("Unable to write inode blocks [init_dev]");
            free(block_buffer);
            return -1;
        }

        // root inode, represents the "/" directory and super_block
        // size will be incremented as directories/files are created within
        // the root
        inode sb_inode = {
            .size = sizeof(super_block),
            .type = IS_DIR_INODE,
            .uid = 0,
            .gid = 0,
            .last_accessed = 0,
            .last_changed = 0,
            .created_at = (u_int64_t) time(NULL),
        };

        memset(sb_inode.pointers, 0, (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES));
        if (write_inode(fd, &sb_inode, 0) == -1) {
            pprintf("Unable to write inode [init_dev]");
            free(block_buffer);
            return -1;
        }

        // write indirect node blocks
        if (initialize_indirect_node_blocks(fd) == -1) {
            pprintf("Unable to write indirect node blocks [init_dev]");
            free(block_buffer);
            return -1;
        }
    }

    // initialize bitmaps here

    free(block_buffer);

    return 1;
}

/**
 * @brief Closes the opened block device file
 * 
 * @param fd File descriptor
 */
void close_dev(int * fd) {
    if (fd == NULL || *fd == -1) {
        pprintf("Invalid parameters provided [init_dev]");
        return;
    }

    close(*fd);

    return;
}
