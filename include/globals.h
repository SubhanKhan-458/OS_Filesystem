#ifndef __GLOBALS__
#define __GLOBALS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <unistd.h>
#include <fcntl.h>

#define MB_TO_BYTES(n) ((int) (n * 1024 * 1024))
#define KB_TO_BYTES(n) ((int) (n * 1024))

#define BLOCK_DEVICE_SIZE (MB_TO_BYTES(512)) // 512 MB
#define BLOCK_SIZE (KB_TO_BYTES(4)) // 4 KB

#define TOTAL_NO_OF_BLOCKS ((int) (BLOCK_DEVICE_SIZE / BLOCK_SIZE))

#define TOTAL_NO_OF_INODE_BLOCKS (((int) (0.0015 * TOTAL_NO_OF_BLOCKS)) + 1) // add 1 additional block
#define NO_OF_INODES_PER_BLOCK(size_of_inode) ((int) (BLOCK_SIZE / size_of_inode))
#define TOTAL_NO_OF_INODES(size_of_inode) ((int) (TOTAL_NO_OF_INODE_BLOCKS * NO_OF_INODES_PER_BLOCK(size_of_inode)))

#define NO_OF_DIRECT_INDEXES 10 // Indexes that point directly to data blocks
#define NO_OF_INDIRECT_INDEXES 2 // Indexes that point to an indirect node

#define NO_OF_DENTRY_PER_BLOCK(size_of_dentry) ((int) (BLOCK_SIZE / size_of_dentry))
#define TOTAL_NO_OF_DENTRY_BLOCKS(size_of_inode, size_of_dentry) (((int) (TOTAL_NO_OF_INODES(size_of_inode) / NO_OF_DENTRY_PER_BLOCK(size_of_dentry))) + 1) // add 1 additional block

#define NO_OF_INDIRECT_NODES_PER_BLOCK(size_of_indirect_node) ((int) (BLOCK_SIZE / size_of_indirect_node))
#define TOTAL_NO_OF_INDIRECT_NODES(size_of_inode) ((int) (TOTAL_NO_OF_INODES(size_of_inode) * NO_OF_INDIRECT_INDEXES))
#define TOTAL_NO_OF_INDIRECT_NODE_BLOCKS(size_of_inode, size_of_indirect_node) (((int) (TOTAL_NO_OF_INDIRECT_NODES(size_of_inode) / NO_OF_INDIRECT_NODES_PER_BLOCK(size_of_indirect_node))) + 1)  // add 1 additional block

#define TOTAL_NO_OF_DIRECT_BLOCKS

#define MAX_FILENAME_LENGTH 32

#define FS_MAGIC_NUMBER 0x534E5300

#define SUPER_BLOCK_INDEX_NO 0
#define DENTRY_BLOCKS_INDEX_NO 1
#define INODE_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry) (DENTRY_BLOCKS_INDEX_NO + TOTAL_NO_OF_DENTRY_BLOCKS(size_of_inode, size_of_dentry))
#define INDIRECT_NODE_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry) (INODE_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry) + TOTAL_NO_OF_INODE_BLOCKS)

#define pprintf(content) (printf("%s\n", content))

typedef struct super_block {
    u_int32_t magic_number;
    u_int32_t total_blocks;
    u_int32_t total_inode_blocks;
    u_int32_t total_no_of_inodes;
} super_block;

typedef struct dentry {
    char filename[MAX_FILENAME_LENGTH];
    int32_t inode_index;
} dentry;

typedef struct inode {
    int32_t size;
    u_int32_t type;
    u_int32_t uid;
    u_int32_t gid;
    u_int64_t last_accessed;
    u_int64_t last_changed;
    u_int64_t created_at;
    int32_t pointers[NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES];
} inode;

typedef struct indirect_node {
    int32_t pointers[NO_OF_DIRECT_INDEXES];
} indirect_node;

/* lib/blocks.c */
int read_block(int *, void *, int);
int write_block(int *, void *, int);
int clean_block(int *, int);
int clean_all_blocks(int *);

/* lib/inodes.c */

#endif
