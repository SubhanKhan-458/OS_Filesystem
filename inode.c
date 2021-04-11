#include<inode.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

// MOVE TO HEADERFILE
#define BLOCK_SIZE 4 // 4 -> 4096 => 4KB Block
#define MAX_FILE_SIZE (BLOCK_SIZE * 5) // 5 Blocks = 4960 * 5
#define MAX_FILENAME_SIZE 30

off_t getByteOffset(int n) {
    return n * BLOCK_SIZE;
}

// MOVE TO HEADERFILE
typedef struct file {
    char filename[MAX_FILENAME_SIZE];
    char cache[MAX_FILE_SIZE];
    int descriptor;
} file;

void openDisk(file * fileStruct, int nBytes) {
    // if file does not exist, create file
    if ((fileStruct->descriptor = open(fileStruct->filename, O_RDWR)) == -1) {
        perror("fd");
        exit(1);
    }

    // filesystem => mark this file as open
}

void readBlock(file * fileStruct, int blockNo) {
    // TODO: swap with lseek
    if (fileStruct->descriptor < 0) {
        perror("disk");
        exit(1);
    }

    if (blockNo < 0) {
        blockNo = 0;
    }

    int retVal;
    int i = 0;
   
    while ((retVal = read(fileStruct->descriptor, (void *) fileStruct->cache, MAX_FILE_SIZE)) != -1, i < blockNo) {
        printf("A = %s\n", (char *) fileStruct->cache);
        i++;
    }
}

int writeBlock(file * fileStruct, int blockNo, void * block) {
    if (fileStruct->descriptor < 0) {
        perror("disk");
        exit(1);
    }

    int NO_OF_BYTES;
    if (( NO_OF_BYTES = lseek(fileStruct->descriptor, 0, SEEK_END)) == -1) {
        perror("lseek");
        exit(1);
    }

    off_t offset = getByteOffset(blockNo);

    if (NO_OF_BYTES > offset) {
        if (( NO_OF_BYTES = lseek(fileStruct->descriptor, offset, SEEK_SET) == -1)) {
            perror("lseek");
            exit(1);
        }
    }

    printf("%d\n", NO_OF_BYTES);

    return write(fileStruct->descriptor, block, BLOCK_SIZE);
}

int main () {
    file * testFile = (file *) malloc(sizeof(file) * 1);
   
    char * filename = "foo.txt";
    memcpy(testFile->filename, filename, strlen(filename));
 
    openDisk(testFile, 4);
    printf("%d\n", testFile->descriptor);
   
    char * block = (char *) malloc(BLOCK_SIZE * sizeof(char));
    memcpy(block, "BELO", 4);

    // readBlock(testFile, -1);

    // printf("%s\n", testFile->cache);

    writeBlock(testFile, 120, (void *) block);

    // free(block);

    // close(fd);

    if (testFile->descriptor >= 0) close(testFile->descriptor);

    return 0;
}