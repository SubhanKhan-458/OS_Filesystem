#ifndef _GLOBALS_
#define _GLOBALS_

#define BLOCK_SIZE 4096 // 4 KB IN BYTES
#define DISK_SIZE (512 * 1024 * 1024) // 512 MB IN BYTES
#define NO_OF_BLOCKS (DISK_SIZE / BLOCK_SIZE)
#define NO_OF_INODES (NO_OF_BLOCKS * 0.05) // 5% of total blocks

#include <stdlib.h>

#endif
