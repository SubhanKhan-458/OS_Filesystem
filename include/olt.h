// Object Location Table
// Contains bitmaps

#ifndef _OLT_
#define _OLT_

#include "globals.h"
#include "inodes.h"

// TODO: Initialize with -1 when registering filesystem

// Both bitmaps are in-memory, when registering fs
// the number of free blocks is read and stored
struct inode_bitmap {
    char bitmap[GET_NO_OF_INODES(sizeof(inode))];
};

typedef inode_bitmap inode_bitmap;

// TODO: determine size
// struct data_bitmap {
//     char bitmap[]
// };

#endif
