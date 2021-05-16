#ifndef __GLOBALS__
#define __GLOBALS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>

#define MB_TO_BYTES(n) ((int) (n * 1024 * 1024))
#define KB_TO_BYTES(n) ((int) (n * 1024))

#define BLOCK_DEVICE_SIZE (MB_TO_BYTES(10)) // 512 MB
#define BLOCK_SIZE (KB_TO_BYTES(4)) // 4 KB

#define TOTAL_NO_OF_BLOCKS ((int) (BLOCK_DEVICE_SIZE / BLOCK_SIZE))

#define TOTAL_NO_OF_INODE_BLOCKS (((int) (0.0015 * TOTAL_NO_OF_BLOCKS)) + 1) // add 1 additional block
#define NO_OF_INODES_PER_BLOCK(size_of_inode) ((int) (BLOCK_SIZE / size_of_inode))
#define TOTAL_NO_OF_INODES(size_of_inode) ((int) (TOTAL_NO_OF_INODE_BLOCKS * NO_OF_INODES_PER_BLOCK(size_of_inode)))

#define NO_OF_DIRECT_INDEXES 10 // Indexes that point directly to data blocks
#define NO_OF_INDIRECT_INDEXES 2 // Indexes that point to an indirect node

#define NO_OF_DENTRY_PER_BLOCK(size_of_dentry) ((int) (BLOCK_SIZE / size_of_dentry))
#define TOTAL_NO_OF_DENTRY_BLOCKS(size_of_inode, size_of_dentry) (((int) (TOTAL_NO_OF_INODES(size_of_inode) / NO_OF_DENTRY_PER_BLOCK(size_of_dentry))) + 1) // add 1 additional block
#define TOTAL_NO_OF_DENTRY(size_of_inode, size_of_dentry) (((int) (TOTAL_NO_OF_DENTRY_BLOCKS(size_of_inode, size_of_dentry) * NO_OF_DENTRY_PER_BLOCK(size_of_dentry))))

#define NO_OF_INDIRECT_NODES_PER_BLOCK(size_of_indirect_node) ((int) (BLOCK_SIZE / size_of_indirect_node))
#define TOTAL_NO_OF_INDIRECT_NODES(size_of_inode) ((int) (TOTAL_NO_OF_INODES(size_of_inode) * NO_OF_INDIRECT_INDEXES))
#define TOTAL_NO_OF_INDIRECT_NODE_BLOCKS(size_of_inode, size_of_indirect_node) (((int) (TOTAL_NO_OF_INDIRECT_NODES(size_of_inode) / NO_OF_INDIRECT_NODES_PER_BLOCK(size_of_indirect_node))) + 1)  // add 1 additional block

#define TOTAL_NO_OF_DATA_BLOCKS(size_of_inode, size_of_dentry, size_of_indirect_node) ((int) (TOTAL_NO_OF_BLOCKS - (TOTAL_NO_OF_INODE_BLOCKS + TOTAL_NO_OF_DENTRY_BLOCKS(size_of_inode, size_of_dentry) + TOTAL_NO_OF_INDIRECT_NODE_BLOCKS(size_of_inode, size_of_indirect_node) + 1))) // + 1 for superblock

#define SIZEOF_INODE sizeof(inode)
#define SIZEOF_DENTRY sizeof(dentry)
#define SIZEOF_INDIRECT_NODE sizeof(indirect_node)

#define MAX_FILENAME_LENGTH 32
#define MAX_DIR_DEPTH 10
#define MAX_CHILD_INODE_COUNT ((int) (BLOCK_SIZE / 4))

#define FS_MAGIC_NUMBER 0x534E5300

#define IS_EMPTY_INODE 0
#define IS_DIR_INODE 1
#define IS_FILE_INODE 2

#define SUPER_BLOCK_INDEX_NO 0
#define DENTRY_BLOCKS_INDEX_NO 1
#define INODE_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry) (DENTRY_BLOCKS_INDEX_NO + TOTAL_NO_OF_DENTRY_BLOCKS(size_of_inode, size_of_dentry))
#define INDIRECT_NODE_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry) (INODE_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry) + TOTAL_NO_OF_INODE_BLOCKS)
#define DATA_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry, size_of_indirect_node) (INDIRECT_NODE_BLOCKS_INDEX_NO(size_of_inode, size_of_dentry) + TOTAL_NO_OF_INDIRECT_NODE_BLOCKS(size_of_inode, size_of_indirect_node))

#define pprintf(content) (printf("%s\n", content))

typedef struct super_block {
    u_int32_t magic_number;
    u_int32_t total_blocks;
    u_int32_t total_inode_blocks;
    u_int32_t total_no_of_inodes;
} super_block;

typedef struct dentry {
    char filename[MAX_FILENAME_LENGTH];
    int32_t inode_index;
} dentry;

typedef struct inode {
    int32_t size;
    u_int32_t type;
    u_int32_t uid;
    u_int32_t gid;
    u_int64_t last_accessed;
    u_int64_t last_changed;
    u_int64_t created_at;
    int32_t pointers[NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES];
} inode;

typedef struct indirect_node {
    int32_t pointers[NO_OF_DIRECT_INDEXES];
} indirect_node;

typedef struct file {
    char path[(MAX_FILENAME_LENGTH * MAX_DIR_DEPTH) + MAX_FILENAME_LENGTH];
    char name[MAX_FILENAME_LENGTH];
    int32_t inode_index;
} file;

u_int8_t data_blocks_bitmap[TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE)];
u_int8_t inodes_bitmap[TOTAL_NO_OF_INODES(SIZEOF_INODE)];
u_int8_t indirect_nodes_bitmap[TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE)];

file _file;

/* lib/blocks.c */
int read_block(int *, void *, int);
int write_block(int *, void *, int);
int clean_block(int *, int);
int clean_all_blocks(int *);

/* lib/inodes.c */
int initialize_inode_blocks(int *);
int write_inode(int *, inode *, int);
int read_inode(int *, inode *, int);
int clean_inode(int *, int);
int add_inode(int *, inode *);
void dump_inode(inode *);

/* lib/indirect_nodes.c */
int initialize_indirect_node_blocks(int *);
int add_pointer_to_indirect_node(int *, int, int, int);
int remove_pointer_from_indirect_node(int *, int, int);
int write_indirect_node(int *, indirect_node *, int);
int read_indirect_node(int *, indirect_node *, int);
int clean_indirect_node(int *, int);
void dump_indirect_node(indirect_node *);

/* lib/dentry.c */
int initialize_dentry_blocks(int *);
int write_dentry(int *, dentry *, int);
int read_dentry(int *, dentry *, int);
int clean_dentry(int *, int);
int djb2_hash(char *);
int dentry_lookup(int *, char *, int);
int dentry_lookup_with_index(int *, char *, int);
int add_dentry(int *, char *, int);
void dump_dentry(dentry *);

/* lib/data_blocks.c */
int write_data(int *, char *, int, int);
int fill_used_blocks(int *, char *, int, inode *);
int write_remaining_buffer_to_block(int *, char *, char *, int, int *);
int fill_free_blocks(int *, char *, int, inode *, int);
int write_full_buffer_to_block(int *, char *, int, int *, int *);
int fill_used_block_with_partition(int *, char *, char *, inode *, int *);
int partition_and_write(int *, char *, char *, int *, inode *, int *);
int get_remaining_byte_count(int *, int);
int read_data_by_block(int *, char *, inode *, int);

/* lib/olt.c */
int initialize_bitmaps(int *);
int mark_data_block(int *, char *, int);
int get_free_inode_index();
int set_inode_bitmap_value(int, int);
int get_free_indirect_node_index();
int set_indirect_node_bitmap_value(int, int);
int get_free_data_block_index();
int get_data_block_bitmap_value(int);
int set_data_block_bitmap_value(int, int);
void dump_inode_bitmap();
void dump_indirect_node_bitmap();
void dump_data_block_bitmap();

#endif
