#ifndef _FILE_
#define _FILE_

#include "globals.h"

#define MAX_FILENAME_LENGTH 30

struct file_obj {
    char filename[MAX_FILENAME_LENGTH];
    u_int32_t index_node;
};

typedef struct file_obj file_obj;

#endif
