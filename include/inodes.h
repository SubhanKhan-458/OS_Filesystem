#ifndef _INODES_
#define _INODES_

#include "globals.h"

#define IS_A_DIRECTORY 0
#define IS_A_FILE 1

// Indexed Allocation
#define DIRECT_BLOCKS 10
#define INDIRECT_DEPTH 2 // SINGLE INDIRECT + DOUBLE INDIRECT
#define INDIRECT_BLOCKS_N 64

// MAX_FILE_SIZE = 10 Direct Blocks (4096 * 10) + 
//                  1 Single Indirect Block (4096 * n) +
//                  1 Double Indirect Block (4096 * (n * n))
//                  = 160

// INODE SIZE = 88 BYTES
// BLOCK SIZE / INODE SIZE = 46 INODES IN ONE BLOCK
// INODES IN ONE BLOCK * (5% OF TOTAL BLOCKS) = 301,438 INODES IN 5% OF TOTAL BLOCKS
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

// stored as a node, rather than a whole block
struct double_indirect_block {
    u_int32_t index_pointers[2]; // Offset for 2 single indirect pointers
};

#endif
