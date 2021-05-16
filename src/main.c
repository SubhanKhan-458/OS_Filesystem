/**
 * @file main.c
 * @author Shaheer Ahmed (k190233@nu.edu.pk)
 * @brief The entry point for our FS
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/globals.h"

void dump_to_file(const char *, const char *, char *, int, int, int);
int init_dev(const char *, int *);
void close_dev(int *);

int main () {
    int fd;

    printf("TOTAL_NO_OF_BLOCKS = %d\n", TOTAL_NO_OF_BLOCKS);
    printf("TOTAL_NO_OF_INODE_BLOCKS = %d\n", TOTAL_NO_OF_INODE_BLOCKS);
    printf("TOTAL_NO_OF_INDIRECT_NODE_BLOCKS = %d\n", TOTAL_NO_OF_INDIRECT_NODE_BLOCKS(SIZEOF_INODE, SIZEOF_INDIRECT_NODE));
    printf("TOTAL_NO_OF_DENTRY_BLOCKS = %d\n", TOTAL_NO_OF_DENTRY_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("TOTAL_NO_OF_DATA_BLOCKS = %d\n", TOTAL_NO_OF_DATA_BLOCKS(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE));
    printf("TOTAL_NO_OF_INODES = %d\n", TOTAL_NO_OF_INODES(SIZEOF_INODE));
    printf("TOTAL_NO_OF_INDIRECT_NODES = %d\n", TOTAL_NO_OF_INDIRECT_NODES(SIZEOF_INODE));
    printf("TOTAL_NO_OF_DENTRY = %d\n", TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("NO_OF_INODES_PER_BLOCK = %d\n", NO_OF_INODES_PER_BLOCK(SIZEOF_INODE));
    printf("NO_OF_INDIRECT_NODES_PER_BLOCK = %d\n", NO_OF_INDIRECT_NODES_PER_BLOCK(SIZEOF_INDIRECT_NODE));
    printf("NO_OF_DENTRY_PER_BLOCK = %d\n", NO_OF_DENTRY_PER_BLOCK(SIZEOF_DENTRY));
    printf("SUPER_BLOCK_INDEX_NO = %d\n", SUPER_BLOCK_INDEX_NO);
    printf("DENTRY_BLOCKS_INDEX_NO = %d\n", DENTRY_BLOCKS_INDEX_NO);
    printf("INODE_BLOCKS_INDEX_NO = %d\n", INODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("INDIRECT_NODE_BLOCKS_INDEX_NO = %d\n", INDIRECT_NODE_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY));
    printf("DATA_BLOCKS_INDEX_NO = %d\n", DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE));


    init_dev("/home/shaheer/OS_Filesystem/temp/foo.img", &fd);

    inode temp_inode;
    // if (read_inode(&fd, &temp_inode, 0) != -1) {
    //     dump_inode(&temp_inode);
    // }
    
    indirect_node temp_indirect_node;
    // if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
    //     dump_indirect_node(&temp_indirect_node);
    //     if (add_pointer_to_indirect_node(&fd, 0, 0, DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) + 1) != -1) {
    //         if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
    //             dump_indirect_node(&temp_indirect_node);
    //         }
    //     }

    //     if (add_pointer_to_indirect_node(&fd, 0, 9, DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) + 8) != -1) {
    //         if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
    //             dump_indirect_node(&temp_indirect_node);
    //         }
    //     }

    //     if (remove_pointer_from_indirect_node(&fd, 0, 0) != -1) {
    //         if (read_indirect_node(&fd, &temp_indirect_node, 0) != -1) {
    //             dump_indirect_node(&temp_indirect_node);
    //         }
    //     }
    // }

    // if (read_indirect_node(&fd, &temp_indirect_node, 1) != -1) {
    //     dump_indirect_node(&temp_indirect_node);
    // }

    // if (read_indirect_node(&fd, &temp_indirect_node, 360) != -1) {
    //     dump_indirect_node(&temp_indirect_node);
    // }

    // dentry temp_dentry;
    // if (read_dentry(&fd, &temp_dentry, 0) != -1) {
    //     dump_dentry(&temp_dentry);
    // }

    // if (read_dentry(&fd, &temp_dentry, 226) != -1) {
    //     dump_dentry(&temp_dentry);
    // }

    // if (read_dentry(&fd, &temp_dentry, 184) != -1) {
    //     dump_dentry(&temp_dentry);
    // }
    
    dump_inode_bitmap();
    dump_indirect_node_bitmap();

    int lim = 50;
    dump_data_block_bitmap(&lim);

    // 4096 BYTES
    char * MSG1 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo dolor, tempus non, auctor et, hendrerit quis, nisi. Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis. Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a est. Phasellus magna. In hac habitasse platea dictumst. Curabitur at lacus ac velit ornare lobortis. Cur";
    // 4200 BYTES
    char * MSG2 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo dolor, tempus non, auctor et, hendrerit quis, nisi. Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis. Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a est. Phasellus magna. In hac habitasse platea dictumst. Curabitur at lacus ac velit ornare lobortis. Curabitur a felis in nunc fringilla tristique. Morbi mattis ullamcorper velit. Phasellus gravida semper nis";
    // 3992 BYTES
    char * MSG3 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo dolor, tempus non, auctor et, hendrerit quis, nisi. Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis. Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a.";

    if (inodes_bitmap[1] == 0) {
        clean_inode(&fd, 1);
        read_inode(&fd, &temp_inode, 1);
        temp_inode.size = 4096;
        temp_inode.created_at = time(NULL);
        temp_inode.type = IS_FILE_INODE;
        write_inode(&fd, &temp_inode, 1);
        set_inode_bitmap_value(1, 1);
    }

    // TODO: Fix not properly cascading indirect blocks with additional data
    write_data(&fd, MSG1, 4096, 1);
    write_data(&fd, MSG2, 4200, 1);
    write_data(&fd, MSG3, 3992, 1);

    read_indirect_node(&fd, &temp_indirect_node, 0);
    dump_indirect_node(&temp_indirect_node);

    read_indirect_node(&fd, &temp_indirect_node, 1);
    dump_indirect_node(&temp_indirect_node);

    dump_data_block_bitmap(&lim);

    // dump_data_block_bitmap();

    // temp
    char * bl = (char *) malloc(sizeof(char) * BLOCK_SIZE);

    read_inode(&fd, &temp_inode, 1);

    int i;    
    for (i = 0; i < (NO_OF_DIRECT_INDEXES + (NO_OF_DIRECT_INDEXES * NO_OF_INDIRECT_INDEXES)); i++) {
        read_data_by_block(&fd, bl, &temp_inode, i);
        if (i == 0) dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "w", bl, BLOCK_SIZE, 0, 1);
        else dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "a", bl, BLOCK_SIZE, 0, 1);
    }

    // read_block(&fd, (void *) bl, DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) + 14);
    // dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "w", bl, BLOCK_SIZE, 0);

    // free(bl);

    close_dev(&fd);

    return 0;
}

/**
 * @brief Dumps the contents of a buffer to a specified file
 * 
 * It will stop writing to the file when the target size is reached
 * or when a null-terminator ('\0') is found
 * 
 * @param path Path to our dump file
 * @param modes File modes ("r", "w", "a"...)
 * @param data The data we want to dump
 * @param n Size of data (in bytes)
 */
void dump_to_file(const char * path, const char * modes, char * data, int n, int add_space, int sep_at_block) {
    FILE * fp = fopen(path, modes);
    int i;

    if (fp == NULL) {
        pprintf("Unable to open file for dumping... [dump_to_file]");
        return;
    }

    for (i = 0; (i < n) && (data[i] != '\0'); i++) {
        fprintf(fp, "%c", data[i]);
        if (add_space == 1) {
            fprintf(fp, " ");
        }
    }


    if (sep_at_block == 1) {
        fprintf(fp, "\n");
    }

    fclose(fp);

    return;
}

/**
 * @brief Initializes the block device, from creating the device file to 
 * populating the inodes and writing the superblock
 * 
 * Compares the first 4 bytes of the file with the filesystem's magic number, 
 * if it is the same, it only initializes the bitmaps, else it initializes everything
 * 
 * @param path Path to block device (file)
 * @param fd File descriptor (we write the returned fd to this)
 * @return (int) -1 for error, 1 for ok
 */
int init_dev(const char * path, int * fd) {
    if (path == NULL || fd == NULL) {
        pprintf("Invalid parameters provided [init_dev]");
        return -1;
    }

    *fd = open(path, O_CREAT | O_RDWR, S_IRWXU);
    if (*fd == -1) {
        pprintf("Unable to open file [init_dev]");
        perror("open");
        return -1;
    }

    // read first block
    // if first block has superblock which contains correct magic number
    // then no need to initialize inodes and everything, just the bitmaps
    // else initialize everything from scratch
    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [init_dev]");
        perror("malloc");
        return -1;
    }

    if (read_block(fd, (void *) block_buffer, 0) == -1) {
        pprintf("Unable to read_block [init_dev]");
        free(block_buffer);
        return -1;
    }

    super_block sb;

    memcpy(&sb.magic_number, block_buffer, sizeof(__uint32_t));
    
    if (sb.magic_number != FS_MAGIC_NUMBER) {
        // initialize inode blocks, data blocks etc.
        // everything gets initialized from scratch
        // first time

        // clean all the blocks first
        if (clean_all_blocks(fd) == -1) {
            pprintf("Unable to clean all blocks [init_dev]");
            free(block_buffer);
            return -1;
        }

        sb.magic_number = (u_int32_t) FS_MAGIC_NUMBER;
        sb.total_blocks = (u_int32_t) TOTAL_NO_OF_BLOCKS;
        sb.total_inode_blocks = (u_int32_t) TOTAL_NO_OF_INODE_BLOCKS;
        sb.total_no_of_inodes = (u_int32_t) TOTAL_NO_OF_INODES(SIZEOF_INODE);

        memcpy(block_buffer, &sb, sizeof(super_block));

        // write super block
        if (write_block(fd, (void *) block_buffer, SUPER_BLOCK_INDEX_NO) == -1) {
            pprintf("Unable to write super block [init_dev]");
            free(block_buffer);
            return -1;
        }

        // write dentry blocks
        if (initialize_dentry_blocks(fd) == -1) {
            pprintf("Unable to write dentry blocks [init_dev]");
            free(block_buffer);
            return -1;
        }

        // root dentry, represents the "/" directory and root
        dentry sb_dentry = {
            .filename = "/",
            .inode_index = 0,
        };

        if (write_dentry(fd, &sb_dentry, 0) == -1) {
            pprintf("Unable to write dentry [init_dev]");
            free(block_buffer);
            return -1;
        }

        // write inode blocks
        if (initialize_inode_blocks(fd) == -1) {
            pprintf("Unable to write inode blocks [init_dev]");
            free(block_buffer);
            return -1;
        }

        // root inode, represents the "/" directory and super_block
        // size will be incremented as directories/files are created within
        // the root
        inode sb_inode = {
            .size = sizeof(super_block),
            .type = IS_DIR_INODE,
            .uid = 0,
            .gid = 0,
            .last_accessed = 0,
            .last_changed = 0,
            .created_at = (u_int64_t) time(NULL),
        };

        memset(sb_inode.pointers, 0, (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES));
        if (write_inode(fd, &sb_inode, 0) == -1) {
            pprintf("Unable to write inode [init_dev]");
            free(block_buffer);
            return -1;
        }

        // write indirect node blocks
        if (initialize_indirect_node_blocks(fd) == -1) {
            pprintf("Unable to write indirect node blocks [init_dev]");
            free(block_buffer);
            return -1;
        }
    }

    // initialize bitmaps here
    // inode_bitmap initializes the rest of the bitmaps along with it
    if (initialize_bitmaps(fd) == -1) {
        pprintf("Unable to initialize inode bitmap [init_dev]");
        free(block_buffer);
        return -1;
    }

    free(block_buffer);

    return 1;
}

/**
 * @brief Closes the opened block device file
 * 
 * @param fd File descriptor
 */
void close_dev(int * fd) {
    if (fd == NULL || *fd == -1) {
        pprintf("Invalid parameters provided [close_dev]");
        return;
    }

    close(*fd);

    return;
}

int get_tokens(char * str, char * delim, char * tokens_buffer[], int * dir_depth) {
    if (str == NULL || delim == NULL) {
        pprintf("Invalid parameters provided [get_token]");
        return -1;
    }

    char * token;
    int i = 0;
    
    while (( token = strtok(str, delim)) != NULL) {
        (*dir_depth)++;
        strcmp(tokens_buffer[i++], token);
    }

    return 1;
}

int alloc_delimited_path(char * delimited_path[]) {
    int i;

    for (i = 0; i < MAX_DIR_DEPTH; i++) {
        delimited_path[i] = (char *) malloc(sizeof(char) * MAX_FILENAME_LENGTH);
        if (delimited_path[i] == NULL) {
            pprintf("Unable to allocate memory [alloc_delimited_path]");
            return -1;
        }
    }

    return 1;
}

void dealloc_delimited_path(char * delimited_path[]) {
    int i;

    for (i = 0; i < MAX_DIR_DEPTH; i++) {
        if (delimited_path[i] == NULL) {
            continue;
        }

        free(delimited_path[i]);
    }
}

int load_inodes_indexes_to_array(char * block_buffer, int32_t * inodes_in_dir, int * inode_index_count) {
    if (block_buffer == NULL || inodes_in_dir == NULL) {
        pprintf("Invalid parameters provided [load_inodes_indexes_to_array]");
        return -1;
    }

    *inode_index_count = 0;

    while (strncmp(block_buffer, "\0\0\0\0", sizeof(char) * 4) != 0) {
        memcpy(inodes_in_dir[(*inode_index_count)++], block_buffer + ((*inode_index_count) * sizeof(char) * 4), sizeof(char) * 4);
    }

    return 1;
}

int snsfs_mknod(int * fd, char * name, int type) {
    if (fd == NULL || *fd < 1 || name == NULL) {
        pprintf("Invalid parameters provided [snsfs_mknod]");
        return -1;
    }

    if (type < IS_EMPTY_INODE || type > IS_FILE_INODE) {
        pprintf("Invalid inode type provided [snsfs_mknod]");
        return -1;
    }

    inode temp;
    temp.type = type;

    int inode_index = add_inode(fd, &temp);
    if (inode_index == -1) {
        pprintf("Unable to create inode [snsfs_mknod]");
        return -1;
    }

    if (add_dentry(fd, name, inode_index) == -1) {
        pprintf("Unable to create dentry [snsfs_mknod]");
        return -1;
    }

    return inode_index;
}

// only support absolute paths
int snsfs_cd(int * fd, char * dir_path) {
    if (fd == NULL || *fd == -1 || dir_path == NULL) {
        pprintf("Invalid parameters provided [snsfs_cd]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [snsfs_cd]");
        return -1;
    }

    inode temp;

    // load root's inodes
    if (read_inode(fd, &temp, 0) == -1) {
        free(block_buffer);
        return -1;
    }

    // read first data block of root node
    if (read_block(fd, &temp, temp.pointers[0]) == -1) {
        free(block_buffer);
        return -1;
    }

    int32_t inodes_in_dir[MAX_CHILD_INODE_COUNT];
    int inode_count = 0;

    // copy 4 bytes into inodes_in_dir till null-terminators are found
    if (load_inodes_indexes_to_array(block_buffer, inodes_in_dir, &inode_count) == -1) {
        free(block_buffer);
        return -1;
    }

    char * node_name;
    int inode_index, i, j;
    int inode_found;

    while ((node_name = strsep(&dir_path, "/")) != NULL) {
        // skip root
        if (strcmp(node_name, "/") == 0) {
            continue;
        }

        inode_found = 0, j = 0;

        while (j < inode_count) {
            // actual index is obtained by subtracting 1
            inode_index = (dentry_lookup(fd, node_name, j) - 1);
            j++;

            if (inode_index == -1) {
                free(block_buffer);
                return -1;
            }

            for (i = 0; i < inode_count; i++) {
                if (inodes_in_dir[i] == inode_index) {
                    // load target inode's child inodes
                    if (read_inode(fd, &temp, inode_index) == -1) {
                        free(block_buffer);
                        return -1;
                    }

                    if (temp.type != IS_DIR_INODE) {
                        free(block_buffer);
                        return -1;
                    }

                    // read first data block of target inode
                    if (read_block(fd, &temp, temp.pointers[0]) == -1) {
                        free(block_buffer);
                        return -1;
                    }

                    inode_found = 1;
                    break;
                }
            }

            if (inode_found == 1) {
                break;
            }
        }

        if (inode_found == 0) {
            free(block_buffer);
            return -1;
        }
    }

    strcpy(_file.path, dir_path);
    strcpy(_file.name, node_name);

    printf("[SUCCESS] the current directory is {%s}\n", _file.path);

    return 1;
}

// TODO: mkdir
int snsfs_mkdir(int * fd, char * dir_path) {
    if (fd == NULL || *fd == -1 || dir_path == NULL) {
        pprintf("Invalid parameters provided [snsfs_mkdir]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        pprintf("Unable to allocate memory [snsfs_mkdir]");
        return -1;
    }

    inode temp;

    // load root's inodes
    if (read_inode(fd, &temp, 0) == -1) {
        free(block_buffer);
        return -1;
    }

    // read first data block of root node
    if (read_block(fd, &temp, temp.pointers[0]) == -1) {
        free(block_buffer);
        return -1;
    }

    int32_t inodes_in_dir[MAX_CHILD_INODE_COUNT];
    int inode_count = 0;

    // copy 4 bytes into inodes_in_dir till null-terminators are found
    if (load_inodes_indexes_to_array(block_buffer, inodes_in_dir, &inode_count) == -1) {
        free(block_buffer);
        return -1;
    }

    char * node_name;
    int dir_inode_index = 0, inode_index, i, j;
    int inode_found;

    while ((node_name = strsep(&dir_path, "/")) != NULL) {
        // skip root
        if (strcmp(node_name, "/") == 0) {
            continue;
        }

        // max number of childs reached
        if (inode_count >= MAX_CHILD_INODE_COUNT) {
            free(block_buffer);
            return -1;
        }

        inode_found = 0, j = 0;

        while (j < inode_count) {
            inode_index = (dentry_lookup(fd, node_name, j) - 1);
            j++;

            // directory doesn't exist
            if (inode_index == -1) {
                inode_index = snsfs_mknod(fd, node_name, IS_DIR_INODE);
                if (inode_index == -1) {
                    free(block_buffer);
                    return -1;
                }

                // created dir
                if (add_inode_to_dir(fd, dir_inode_index, inode_index) == -1) {
                    free(block_buffer);
                    return -1;
                }
            }

            for (i = 0; i < inode_count; i++) {
                if (inodes_in_dir[i] == inode_index) {
                    // load target inode's child inodes
                    if (read_inode(fd, &temp, inode_index) == -1) {
                        free(block_buffer);
                        return -1;
                    }

                    if (temp.type != IS_DIR_INODE) {
                        free(block_buffer);
                        return -1;
                    }

                    // read first data block of target inode
                    if (read_block(fd, &temp, temp.pointers[0]) == -1) {
                        free(block_buffer);
                        return -1;
                    }

                    inode_found = 1;
                    break;
                }
            }

            if (inode_found == 1) {
                break;
            }
        }

        if (inode_found == 0) {
            free(block_buffer);
            return -1;
        }
    }

    strcpy(_file.path, dir_path);
    strcpy(_file.name, node_name);

    printf("[SUCCESS] the current directory is {%s}\n", _file.path);

    return 1;
}
