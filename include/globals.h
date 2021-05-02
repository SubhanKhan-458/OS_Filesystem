#ifndef _GLOBALS_
#define _GLOBALS_

#include <stdlib.h>

#define MB_TO_BYTES(MB) ((int) MB * 1024 * 1024)
#define KB_TO_BYTES(KB) ((int) KB * 1024)
#define BLOCK_SIZE (KB_TO_BYTES(4)) // 4 KB IN BYTES
#define DISK_SIZE (MB_TO_BYTES(512)) // 512 MB IN BYTES
#define NO_OF_BLOCKS ((int) (DISK_SIZE / BLOCK_SIZE)) // 131,072 blocks
#define NO_OF_INODE_BLOCKS ((int) (NO_OF_BLOCKS * 0.0015)) // 0.15% of total blocks
#define GET_NO_OF_INODES(sizeof_inode) ((int) ((BLOCK_SIZE / sizeof_inode) * NO_OF_INODE_BLOCKS))
#define GET_INODES_PER_BLOCK(sizeof_inode) ((int) (BLOCK_SIZE / sizeof_inode))

#define SUPERBLOCK_BLOCK_NO 0
#define FILE_MAP_BLOCK_NO 1 // 1 - 76 = 75
#define INODE_BLOCK_NO 77 // 77 - 273 = 196
#define INDEX_BLOCK_NO 274 // 274 - 450 = 176
#define DATA_BLOCK_NO 451

#define NO_OF_DATA_BLOCKS (NO_OF_BLOCKS - DATA_BLOCK_NO)

#endif
