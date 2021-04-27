#ifndef _GLOBALS_
#define _GLOBALS_

#include <stdlib.h>

#define MB_TO_BYTES(MB) ((int) MB * 1024 * 1024)
#define KB_TO_BYTES(KB) ((int) KB * 1024)
#define BLOCK_SIZE 4 // 4 KB IN BYTES
#define DISK_SIZE (MB_TO_BYTES(512)) // 512 MB IN BYTES
#define NO_OF_BLOCKS ((int) (DISK_SIZE / BLOCK_SIZE)) // 131,072 blocks
#define NO_OF_INODE_BLOCKS ((int) (NO_OF_BLOCKS * 0.0015)) // 0.15% of total blocks
#define GET_NO_OF_INODES(sizeof_inode) ((int) ((BLOCK_SIZE / sizeof_inode) * NO_OF_INODE_BLOCKS))
#define SUPERBLOCK_BLOCK_NO 0

#endif
