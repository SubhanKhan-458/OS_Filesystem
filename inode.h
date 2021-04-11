#ifndef _DISK_H_
#define _DISK_H_


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include <fcntl.h>

// total disk size = 512MB
// total no of blocks = 512M/4096 = 131072
#define DISK_BLOCKS 131072        
#define BLOCK_SIZE 4096           
#define NO_OF_INODES 78644 //60% -> 78644  | 70% -> 87659      
#define NO_OF_FILE_DESCRIPTORS 3
//standard 32 

struct inode // 4Byte
{
    int filesize;
};

struct file_to_inode_mapping // 36Byte
{
    char file_name[30]; 
    int inode_num;      
};

struct super_block 
{
    bool inode_freelist[NO_OF_INODES];    
    bool datablock_freelist[DISK_BLOCKS]; 
    
    //6*2 + 131072 + 78644
    //Blocks = 209740/4096 = 52 blocks 
    int no_of_blocks_used_by_superblock = ceil(((float)sizeof(super_block)) / BLOCK_SIZE);

    //mapping b/w filename and inode is 36*78644/4096 = 692  
    int no_of_blocks_used_by_file_inode_mapping = ceil(((float)sizeof(struct file_to_inode_mapping) * NO_OF_INODES) / BLOCK_SIZE);

    //Inode starts at 744 
    int starting_index_of_inodes = no_of_blocks_used_by_superblock + no_of_blocks_used_by_file_inode_mapping;

    //Size to store all inodes = 78644*52 = 999
    int no_of_blocks_used_to_store_inodes = ceil(((float)(NO_OF_INODES * sizeof(struct inode))) / BLOCK_SIZE); // 8192

    //52 + 692 + 999 = 1743 (reserved blocks) 
    int starting_index_of_data_blocks = no_of_blocks_used_by_superblock + no_of_blocks_used_by_file_inode_mapping + no_of_blocks_used_to_store_inodes;

    /* 131072 - 1743 = 129329 (free blocks to store data) */
    int total_no_of_available_blocks = DISK_BLOCKS - starting_index_of_data_blocks;
};

#endif
