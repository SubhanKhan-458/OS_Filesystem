// Object Location Table
// Contains bitmaps

#ifndef _OLT_
#define _OLT_

#include "globals.h"
#include "inodes.h"

#define NO_OF_INODES ((int) GET_NO_OF_INODES(sizeof(inode)))
#define NO_OF_INDEX_BLOCKS ((int) GET_NO_OF_INODES(sizeof(inode)) * 2)

// TODO: Initialize with '0' when registering filesystem

// Both bitmaps are in-memory, when registering fs
// the number of free blocks is read and stored
struct inode_bitmap {
    char bitmap[NO_OF_INODES];
};

typedef struct inode_bitmap inode_bitmap;

struct index_block_bitmap {
    char bitmap[NO_OF_INDEX_BLOCKS];
};

typedef struct index_block_bitmap index_block_bitmap;

// TODO: determine size
struct data_bitmap {
    char bitmap[NO_OF_DATA_BLOCKS];
};


typedef struct data_bitmap data_bitmap;

inode_bitmap INODE_BITMAP;
index_block_bitmap INDEX_BLOCK_BITMAP;
data_bitmap DATA_BITMAP;

void initialize_inode_bitmap(int * device_descriptor);
void initialize_index_block_bitmap(int * device_descriptor);
void initialize_data_block_bitmap(int * device_descriptor);
int get_empty_inode_index();
int get_empty_index_block_index();
int get_empty_data_block_index();

#endif
