#include "../include/data_blocks.h"
#include "../include/olt.h"
#include<stdlib.h>
#include <stdio.h>

void initialize_data_blocks(int *device_descriptor)
{
    char buffer[1] = {'\0'};
    int i;

    for (i = DATA_BLOCK_NO; i < NO_OF_BLOCKS; i++)
    {
        writeBlock(device_descriptor, (void *)buffer, i);
    }
}

int remaining_block_lookup(int *device_descriptor, inode *inode_buff, int buffer_size, int *remaining_size)
{
    int i, j, datablock_index;
    char read_buff[BLOCK_SIZE];

    for (i = 0; i < DIRECT_BLOCKS; i++)
    {
        datablock_index = inode_buff->index_pointers[i];
        if (datablock_index != -1)
        {
            readBlock(device_descriptor, read_buff, datablock_index);
            j = 0;
            while (read_buff[j] != '\0')
            {
                j++;
            }

            if ((BLOCK_SIZE - j) > buffer_size)
            {
                *remaining_size = j;
                return datablock_index;
            }
        }

        datablock_index = -1;
    }

    single_indirect_block *indirect_node_buff = (single_indirect_block *)malloc(sizeof(single_indirect_block));
    int k;

    for (; i < (DIRECT_BLOCKS + INDIRECT_DEPTH); i++)
    {
        if (inode_buff->index_pointers[i] != -1)
        {
            // fetch single_indirect node
            read_indirect_node(device_descriptor, indirect_node_buff, inode_buff->index_pointers[i]);

            for (k = 0; k < DIRECT_BLOCKS; k++)
            {
                datablock_index = indirect_node_buff->index_pointers[k];
                if (datablock_index != -1)
                {
                    readBlock(device_descriptor, read_buff, datablock_index);
                    j = 0;
                    while (read_buff[j] != '\0')
                    {
                        j++;
                    }

                    if ((BLOCK_SIZE - j) > buffer_size)
                    {
                        *remaining_size = j;
                        return datablock_index;
                    }
                }

                datablock_index = -1;
            }
        }
    }

    free(indirect_node_buff);

    return -1;
}

int free_block_lookup()
{
    int i;

    for (i = 0; i < NO_OF_DATA_BLOCKS; i++)
    {
        if (DATA_BITMAP.bitmap[i] == '0')
        {
            return (DATA_BLOCK_NO + i);
        }
    }

    return -1;
}

void write_data(int *device_descriptor, int inode_index, char *buffer, int buffer_size)
{
    inode *inode_buffer = (inode *)malloc(sizeof(inode));
    char write_buffer[BLOCK_SIZE]; // we portion the buffer in BLOCK_SIZE chunks

    int remaining_bytes = buffer_size, blocks_written = 0, data_block_index;
    int i, j, write_block_flag = 0, empty_indirect_node;

    single_indirect_block *indirect_node_buffer;

    while (remaining_bytes >= BLOCK_SIZE)
    {
        memcpy(write_buffer, buffer + (blocks_written * (BLOCK_SIZE)), (BLOCK_SIZE - 1));
        write_buffer[BLOCK_SIZE] = '\0';

        remaining_bytes -= (BLOCK_SIZE - 1);

        for (i = 0; i < DIRECT_BLOCKS; i++)
        {
            if (inode_buffer->index_pointers[i] == -1)
            {
                data_block_index = free_block_lookup();
                inode_buffer->index_pointers[i] = data_block_index;
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, write_buffer, data_block_index);

                blocks_written++;
                write_block_flag = 1;
                break;
            }
        }

        if (write_block_flag == 1)
        {
            write_block_flag = 0;
            continue;
        }

        for (i; i < INDIRECT_DEPTH + DIRECT_BLOCKS; i++)
        {
            if (inode_buffer->index_pointers[i] == -1)
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                empty_indirect_node = get_empty_index_block_index();
                data_block_index = free_block_lookup();
                indirect_node_buffer->index_pointers[0] = data_block_index;
                write_indirect_node(device_descriptor, indirect_node_buffer, empty_indirect_node);
                inode_buffer->index_pointers[i] = empty_indirect_node;
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, write_buffer, data_block_index);

                blocks_written++;
                write_block_flag = 1;
                free(indirect_node_buffer);
                break;
            }
            else
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                read_indirect_node(device_descriptor, indirect_node_buffer, inode_buffer->index_pointers[i]);
                for (j = 0; j < DIRECT_BLOCKS; j++)
                {
                    if (indirect_node_buffer->index_pointers[j] == -1)
                    {
                        data_block_index = free_block_lookup();
                        writeBlock(device_descriptor, write_buffer, data_block_index);

                        blocks_written++;
                        write_block_flag = 1;
                        break;
                    }
                }

                if (write_block_flag == 1)
                {
                    break;
                }
                free(indirect_node_buffer);
            }
        }

        if (write_block_flag != 1)
        {
            continue; // problem writing
        }
    }

    int *available_bytes = (int *)malloc(sizeof(int));
    char *remaining_buff = (char *)malloc(sizeof(char) * remaining_bytes + 1);
    data_block_index = remaining_block_lookup(device_descriptor, inode_buffer, (remaining_bytes + 1), available_bytes);
    memcpy(remaining_buff, buffer + (blocks_written * BLOCK_SIZE), remaining_bytes);
    remaining_buff[(sizeof(char) * remaining_bytes)] = '\0';
    if (data_block_index != -1)
    {
        readBlock(device_descriptor, write_buffer, data_block_index);
        memcpy(write_buffer + (*available_bytes), remaining_buff, remaining_bytes);
        return;
    }
    else
    {
        for (i = 0; i < DIRECT_BLOCKS; ++i)
        {
            if (inode_buffer->index_pointers[i] == -1)
            {
                data_block_index = free_block_lookup();
                inode_buffer->index_pointers[i] = data_block_index;
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, remaining_buff, data_block_index);
                return;
            }
        }
        for (i; i < DIRECT_BLOCKS + INDIRECT_DEPTH; ++i)
        {

            if (inode_buffer->index_pointers[i] == -1)
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                empty_indirect_node = get_empty_index_block_index();
                data_block_index = free_block_lookup();
                indirect_node_buffer->index_pointers[0] = data_block_index;
                write_indirect_node(device_descriptor, indirect_node_buffer, empty_indirect_node);
                inode_buffer->index_pointers[i] = empty_indirect_node;
                write_inode(device_descriptor, inode_buffer, inode_index);
                writeBlock(device_descriptor, remaining_buff, data_block_index);
                free(indirect_node_buffer);
                return;
            }
            else
            {
                indirect_node_buffer = (single_indirect_block *)malloc(sizeof(single_indirect_block));
                read_indirect_node(device_descriptor, indirect_node_buffer, inode_buffer->index_pointers[i]);
                for (j = 0; j < DIRECT_BLOCKS; j++)
                {
                    if (indirect_node_buffer->index_pointers[j] == -1)
                    {
                        data_block_index = free_block_lookup();
                        indirect_node_buffer->index_pointers[j] = data_block_index;
                        write_indirect_node(device_descriptor, indirect_node_buffer, empty_indirect_node);
                        inode_buffer->index_pointers[i] = empty_indirect_node;
                        write_inode(device_descriptor, inode_buffer, inode_index);        
                        writeBlock(device_descriptor, remaining_buff, data_block_index);
                        free(indirect_node_buffer);
                        return;
                    }
                }
                free(indirect_node_buffer);
            }
        }
    }
}

void read_data(int *device_descriptor, int inode_index, char *buffer, int buffer_size)
{
    inode* buff = (inode*)malloc(sizeof (inode));

    read_inode(device_descriptor,buff,inode_index);
    int i=0;
    for(i;i<DIRECT_BLOCKS;i++){
        if(buff->index_pointers[i]!=-1){
            readBlock(device_descriptor,(void*)buffer,buff->index_pointers[i]);
            free(buff);
            return;
        }
    }


}

int main () {
    int * fd = (int *) malloc(sizeof(int));
    openDisk("foo.txt", fd);


    int i = 0;
    char * block = (char *) malloc(BLOCK_SIZE);

    for (; i < INODE_BLOCK_NO; i++) {
        writeBlock(fd, (void *) block, i);
    }

    initialize_inode_blocks(fd);
    initialize_inode_bitmap(fd);
    initialize_inode_blocks(fd);
    initialize_index_block_bitmap(fd);
    initialize_data_blocks(fd);
    initialize_data_block_bitmap(fd);

    char * buff = "HELLO WORLD!";
    char *readBuff = (char *) malloc(BLOCK_SIZE);
    write_data(fd, 0, buff, 12);
    read_data(fd,0,readBuff,BLOCK_SIZE);

    FILE * f = fopen("newfile.txt","w");
    for (i = 0; i < 4096; i++) fprintf(f,"%c ",readBuff[i]);
    fclose(f);
    printf("\n");
    
    free(readBuff);
    free(block);
    
    closeDisk(fd);

    free(fd);

    return 0;
}