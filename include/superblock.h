#ifndef _SUPERBLOCK_
#define _SUPERBLOCK_

#include "globals.h"
#include "inodes.h"
#include "block.h"

struct superblock { 
    u_int32_t magic_number;
    u_int32_t total_blocks;
    u_int32_t inode_blocks;
    u_int32_t no_of_inodes;
};

typedef struct superblock superblock;

void initialize_superblock(int * descriptor);

#endif
