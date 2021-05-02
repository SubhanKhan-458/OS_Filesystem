// Object Location Table
// Contains bitmaps

#ifndef _OLT_
#define _OLT_

#include "globals.h"
#include "inodes.h"

#define NO_OF_INODES ((int) GET_NO_OF_INODES(sizeof(inode)))

// TODO: Initialize with '0' when registering filesystem

// Both bitmaps are in-memory, when registering fs
// the number of free blocks is read and stored
struct inode_bitmap {
    char bitmap[NO_OF_INODES];
};

typedef struct inode_bitmap inode_bitmap;

struct index_block_bitmap {
    char bitmap[(NO_OF_INODES * 2)];
};

typedef struct index_block_bitmap index_block_bitmap;

inode_bitmap INODE_BITMAP;
index_block_bitmap INDEX_BLOCK_BITMAP;

// TODO: determine size
// struct data_bitmap {
//     char bitmap[]
// };

void initialize_inode_bitmap(int * device_descriptor);

#endif
