#ifndef __GLOBALS__
#define __GLOBALS__

#define MB_TO_BYTES(n) ((int) n * 1024 * 1024)
#define KB_TO_BYTES(n) ((int) n * 1024)

#define BLOCK_DEVICE_SIZE (MB_TO_BYTES(512)) // 512 MB
#define BLOCK_SIZE (KB_TO_BYTES(4)) // 4 KB

#define TOTAL_NO_OF_BLOCKS ((int) (BLOCK_DEVICE_SIZE) / (BLOCK_SIZE))

#define FS_MAGIC_NUMBER 0x534E5300

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>

#define pprintf(content) (printf("%s\n", content))

int read_block(int *, void *, int);
int write_block(int *, void *, int);

#endif
