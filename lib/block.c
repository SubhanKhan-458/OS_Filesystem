/**
 * Reads/Writes Blocks
 */

#include "../include/globals.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// diskID is the descriptor for the open file acting as the disk
void openDisk(char * path, int * diskID) {
    // if file does not exist, exit
    if ((*diskID = open(path, O_RDWR)) == -1) {
        perror("fd");
        exit(1);
    }
}

int readBlock(int * descriptor, void * buffer, int blockNo) {
    if (*descriptor < 0) {
        perror("disk");
        return 0;
    }

    if (blockNo < 0) {
        blockNo = 0;
    }

    ssize_t bytes;
    off_t offset;

    if (( offset = lseek(*descriptor, 0, SEEK_END)) == -1) {
        perror("lseek");
        return 0;
    }

    int totalBlocks = offset / BLOCK_SIZE;
    if (totalBlocks < blockNo) {
        printf("Error: total blocks are less than block no. [readBlock]\n");
        return 0;
    }

    if (( offset = lseek(*descriptor, (blockNo * BLOCK_SIZE), SEEK_SET)) == -1) {
        perror("lseek");
        return 0;
    }

    if (( bytes = read(*descriptor, (void *) buffer, BLOCK_SIZE)) == -1) {
        perror("read");
        return 0;
    }

    return 1;
}

int writeBlock(int * descriptor, void * buffer, int blockNo) {
    if (*descriptor < 0) {
        perror("disk");
        return 0;
    }

    if (blockNo < 0) {
        blockNo = 0;
    }

    ssize_t bytes;
    off_t offset;

    if (( offset = lseek(*descriptor, 0, SEEK_END)) == -1) {
        perror("lseek");
        return 0;
    }

    bytes = blockNo * BLOCK_SIZE;

    if (offset > bytes) {
        if (( offset = lseek(*descriptor, bytes, SEEK_SET) == -1)) {
            perror("lseek");
            return 0;
        }
    }

    if (( bytes = write(*descriptor, (void *) buffer, BLOCK_SIZE)) == -1) {
        perror("write");
        return 0;
    }

    return 1;
}

int closeDisk(int * descriptor) {
    if (close(*descriptor) == -1) {
        return 0;
    }

    return 1;
}

// int main () {
//     char * filename = "foo.txt";
//     int * diskID = (int *) malloc(sizeof(int));
//     openDisk(filename, diskID);
//     printf("DISK ID = %d\n", *diskID);
   
//     char * block = (char *) malloc(BLOCK_SIZE * sizeof(char));

//     // readBlock(diskID, block, -1);
    
//     // for (int i = 0; i < BLOCK_SIZE; i++) {
//     //     printf("%c", block[i]);
//     // }

//     // printf("\n");

//     // memcpy(block, "MELO", 4);
//     // writeBlock(diskID, block, 1);

//     free(block);
//     free(diskID);

//     closeDisk(diskID);

//     return 0;
// }
