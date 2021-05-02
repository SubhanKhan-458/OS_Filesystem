#ifndef _DATA_BLOCKS_
#define _DATA_BLOCKS_

#include <string.h>

#include "block.h"
#include "inodes.h"
#include "olt.h"

void write_data(int * device_descriptor, int inode_index, char * buffer, int buffer_size);

#endif