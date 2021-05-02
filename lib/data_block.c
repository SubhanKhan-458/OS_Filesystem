#include "../include/data_blocks.h"

void write_data(int * device_descriptor, int inode_index, char * buffer, int buffer_size) {
    inode * inode_buffer = (inode *) malloc(sizeof(inode));
    char write_buffer[BLOCK_SIZE]; // we portion the buffer in BLOCK_SIZE chunks

    int remaining_bytes = buffer_size;
    int offset_by = 0;
    int data_block_index, i, write_block_res = -1;
    
    read_inode(device_descriptor, inode_buffer, inode_index);

    while (remaining_bytes > 0) {
        memcpy(write_buffer, buffer + (offset_by), (remaining_bytes > BLOCK_SIZE ? BLOCK_SIZE : remaining_bytes));
        offset_by += (remaining_bytes > BLOCK_SIZE) ? BLOCK_SIZE : remaining_bytes;
        remaining_bytes -= (remaining_bytes > BLOCK_SIZE) ? BLOCK_SIZE : remaining_bytes;

        if ((data_block_index = get_empty_data_block()) < 0) {
            free(inode_buffer);
            return;
        }

        for (i = 0; i < DIRECT_BLOCKS; i++) {
            if (inode_buffer->index_pointers[i] == 0) {
                if ((write_block_res = writeBlock(device_descriptor, (void *) write_buffer, (DATA_BLOCK_NO + data_block_index))) == 0) {
                    free(inode_buffer);
                    return;
                }

                inode_buffer->index_pointers[i] = data_block_index;
                break;
            }
        }

        if (write_block_res < 0) {
            // check single indirects here and assign accordingly
        } else {
            write_block_res = -1;
        }
    }

    // find data block here

}

int get_empty_data_block() {
    int i;

    for (i = 0; i < NO_OF_DATA_BLOCKS; i++) {
        if (DATA_BITMAP.bitmap[i] == '0') {
            return i;
        }
    }

    return -1;
}
