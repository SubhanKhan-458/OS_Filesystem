#ifndef _INODES_
#define _INODES_

#include "globals.h"

#define IS_A_DIRECTORY 0
#define IS_A_FILE 1
#define IS_EMPTY 2

// Indexed Allocation
#define DIRECT_BLOCKS 10
#define INDIRECT_DEPTH 2 // SINGLE INDIRECT + SINGLE INDIRECT
#define INDIRECT_BLOCKS_N 64

// MAX_FILE_SIZE = 10 Direct Blocks (4096 * 10) + 
//                  1 Single Indirect Block (4096 * n) +
//                  1 Single Indirect Block (4096 * n)
//                  = 30

struct inode {
    u_int32_t size; // 4 BYTES
    // u_int32_t index; // 4 BYTES
    mode_t type; // IS_A_DIRECTORY | IS_A_FILE | 4 BYTES
    uid_t uid; // user id | 4 BYTES
    gid_t gno; // group number | 4 BYTES
    time_t last_accessed; // 8 BYTES
    time_t last_changed; // 8 BYTES
    time_t created_at; // 8 BYTES
    u_int32_t index_pointers[DIRECT_BLOCKS + INDIRECT_DEPTH]; // 4 * (10 + 2) BYTES
};

typedef struct inode inode;

// stored as a node, rather than a whole block
struct single_indirect_block {
    u_int32_t index_pointers[DIRECT_BLOCKS]; // 40 BYTES
};

typedef struct single_indirect_block single_indirect_block; // 113 blocks

void initialize_inode_blocks(int * device_descriptor);
void write_inode(int * device_descriptor, inode * buffer, int inode_index);
void read_inode(int * device_descriptor, inode * buffer, int inode_index);
int get_inode_block_no(int inode_index);

#endif
