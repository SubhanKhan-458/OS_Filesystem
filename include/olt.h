// Object Location Table
// Contains bitmaps

#ifndef _OLT_
#define _OLT_

#include "globals.h"

// TODO: Initialize with -1 when registering filesystem

struct inode_bitmap {
    char bitmap[GET_NO_OF_INODES(sizeof(inode))];
};

typedef inode_bitmap inode_bitmap;



#endif
