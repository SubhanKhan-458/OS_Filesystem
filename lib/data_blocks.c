#include "../include/globals.h"

int write_data(int * fd, char * buffer, int inode_index) {

}

void dump_data_block_bitmap() {
    int i;

    printf("--------------------------------------------\n");
    printf("DATA BLOCK BITMAP\n");
    printf("--------------------------------------------\n");

    for (i = 0; i < TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE); i++) {
        printf("{[%d] %d} ", i, data_blocks_bitmap[i]);
    }

    printf("\n");
    printf("--------------------------------------------\n");

    return;
}
