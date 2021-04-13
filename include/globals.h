#ifndef _GLOBALS_
#define _GLOBALS_

#include <stdlib.h>

#define BLOCK_SIZE 4096 // 4 KB IN BYTES
#define DISK_SIZE (512 * 1024 * 1024) // 512 MB IN BYTES
#define NO_OF_BLOCKS ((int) (DISK_SIZE / BLOCK_SIZE)) // 131,072 blocks
#define NO_OF_INODE_BLOCKS ((int) (NO_OF_BLOCKS * 0.01)) // 5% of total blocks
#define GET_NO_OF_INODES(sizeof_inode) ((int) ((BLOCK_SIZE / sizeof_inode) * NO_OF_INODE_BLOCKS))

#endif
