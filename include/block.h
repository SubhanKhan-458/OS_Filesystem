#ifndef _BLOCK_
#define _BLOCK_

void openDisk(char * path, int * diskID);

int readBlock(int * descriptor, char * buffer, int blockNo);

int writeBlock(int * descriptor, char * buffer, int blockNo);

int closeDisk(int * descriptor);

#endif
