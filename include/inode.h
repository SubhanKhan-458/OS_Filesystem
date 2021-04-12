#ifndef _INODES_
#define _INODES_

#define IS_A_DIRECTORY 0
#define IS_A_FILE 1

// Indexed Allocation
#define DIRECT_BLOCKS 10
#define INDIRECT_DEPTH 2 // SINGLE INDIRECT + DOUBLE INDIRECT

// INODE SIZE = 88 BYTES
// BLOCK SIZE / INODE SIZE = 46 INODES IN ONE BLOCK
// INODES IN ONE BLOCK * (5% OF TOTAL BLOCKS) = 301,438 INODES IN 5% OF TOTAL BLOCKS
struct inode {
    u_int32_t size; // 4 BYTES
    // u_int32_t index; // 4 BYTES
    mode_t type; // IS_A_DIRECTORY | IS_A_FILE | 4 BYTES
    uid_t uid; // user id | 4 BYTES
    gid_t gno; // group number | 4 BYTES
    time_t last_accessed; // 8 BYTES
    time_t last_changed; // 8 BYTES
    time_t created_at; // 8 BYTES
    u_int32_t index_pointers[DIRECT_BLOCKS + INDIRECT_DEPTH]; // 4 * (10 + 2) BYTES
};

struct indirect_block {
    u_int32_t index_pointers[DIRECT_BLOCKS + INDIRECT_DEPTH]; // 48 BYTES
};

typedef struct inode inode;

// struct mapping // 36Byte
// {
//     char file_name[30]; 
//     int inode_num;      
// };

// struct superblock 
// {
//     bool inode_list[NO_OF_INODES];    
//     bool datablock_list[DISK_BLOCKS]; 
    
//     //6*2 + 131072 + 78644
//     //Blocks = 209740/4096 = 52 blocks 
//     int no_of_blocks_used_by_superblock = ceil(((float)sizeof(superblock)) / BLOCK_SIZE);

//     //mapping b/w filename and inode is 36*78644/4096 = 692  
//     int no_of_blocks_used_by_file_inode_mapping = ceil(((float)sizeof(struct mapping) * NO_OF_INODES) / BLOCK_SIZE);

//     //Inode starts at 744 
//     int starting_index_of_inodes = no_of_blocks_used_by_superblock + no_of_blocks_used_by_file_inode_mapping;

//     //Size to store all inodes = 78644*52 = 999
//     int no_of_blocks_used_to_store_inodes = ceil(((float)(NO_OF_INODES * sizeof(struct inode))) / BLOCK_SIZE); // 8192

//     //52 + 692 + 999 = 1743 (reserved blocks) 
//     int starting_index_of_data_blocks = no_of_blocks_used_by_superblock + no_of_blocks_used_by_file_inode_mapping + no_of_blocks_used_to_store_inodes;

//     /* 131072 - 1743 = 129329 (free blocks to store data) */
//     int total_no_of_available_blocks = DISK_BLOCKS - starting_index_of_data_blocks;
// };

#endif
