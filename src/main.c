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

int snsfs_mknod(int *, char *, int, int);
int snsfs_cd(int *, char *);
int snsfs_mkdir(int *, char *);
int snsfs_touch(int *, char *);
int snsfs_cat(int *, char *, char *, int);
int snsfs_ls(int *, char *);

// left
int snsfs_head(int *, char *);
int snsfs_tail(int *, char *);
int snsfs_mv(int *, char *, char *);
int snsfs_rm(int *, char *);
int snsfs_cp(int *, char *);

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
    
    // dump_inode_bitmap();
    // dump_indirect_node_bitmap();

    // int lim = 50;
    // dump_data_block_bitmap(&lim);

    // // 4096 BYTES
    // char * MSG1 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo dolor, tempus non, auctor et, hendrerit quis, nisi. Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis. Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a est. Phasellus magna. In hac habitasse platea dictumst. Curabitur at lacus ac velit ornare lobortis. Cur";
    // // 4200 BYTES
    // char * MSG2 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo dolor, tempus non, auctor et, hendrerit quis, nisi. Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis. Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a est. Phasellus magna. In hac habitasse platea dictumst. Curabitur at lacus ac velit ornare lobortis. Curabitur a felis in nunc fringilla tristique. Morbi mattis ullamcorper velit. Phasellus gravida semper nis";
    // // 3992 BYTES
    // char * MSG3 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo dolor, tempus non, auctor et, hendrerit quis, nisi. Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis. Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a.";

    // if (inodes_bitmap[1] == 0) {
    //     clean_inode(&fd, 1);
    //     read_inode(&fd, &temp_inode, 1);
    //     temp_inode.size = 4096;
    //     temp_inode.created_at = time(NULL);
    //     temp_inode.type = IS_FILE_INODE;
    //     write_inode(&fd, &temp_inode, 1);
    //     set_inode_bitmap_value(1, 1);
    // }

    // // TODO: Fix not properly cascading indirect blocks with additional data
    // write_data(&fd, MSG1, 4096, 1);
    // write_data(&fd, MSG2, 4200, 1);
    // write_data(&fd, MSG3, 3992, 1);

    // read_indirect_node(&fd, &temp_indirect_node, 0);
    // dump_indirect_node(&temp_indirect_node);

    // read_indirect_node(&fd, &temp_indirect_node, 1);
    // dump_indirect_node(&temp_indirect_node);

    // dump_data_block_bitmap(&lim);

    // // dump_data_block_bitmap();

    // // temp
    // char * bl = (char *) malloc(sizeof(char) * BLOCK_SIZE);

    // read_inode(&fd, &temp_inode, 1);

    // int i;    
    // for (i = 0; i < (NO_OF_DIRECT_INDEXES + (NO_OF_DIRECT_INDEXES * NO_OF_INDIRECT_INDEXES)); i++) {
    //     read_data_by_block(&fd, bl, &temp_inode, i);
    //     if (i == 0) dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "w", bl, BLOCK_SIZE, 0, 1);
    //     else dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "a", bl, BLOCK_SIZE, 0, 1);
    // }

    // read_block(&fd, (void *) bl, DATA_BLOCKS_INDEX_NO(SIZEOF_INODE, SIZEOF_DENTRY, SIZEOF_INDIRECT_NODE) + 14);
    // dump_to_file("/home/shaheer/OS_Filesystem/temp/.dump", "w", bl, BLOCK_SIZE, 0);

    // free(bl);

    // START

    int i;

    // dentry dentry_buff;
    // for (i = 0; i < TOTAL_NO_OF_DENTRY(SIZEOF_INODE, SIZEOF_DENTRY); i++) {
    //     read_dentry(&fd, &dentry_buff, i);
    //     if (dentry_buff.inode_index > 0) {
    //         dump_dentry(&dentry_buff);
    //         printf("Dentry Index: %d\n", i);
    //     }
    // }

    if (snsfs_mkdir(&fd, "/home/games") == -1) {
        pprintf("MKDIR FAILED");
    }

    // if (snsfs_cd(&fd, "/home/games") == -1) {
    //     pprintf("CD FAILED");
    // }

    // printf("\nuser@ubuntu:%s\n", _file.path);
    // printf("currently opened file: %s\n", _file.name);

    // if (snsfs_cd(&fd, "/games") == -1) {
    //     pprintf("CD FAILED");
    // }

    // printf("\nuser@ubuntu:%s\n", _file.path);
    // printf("currently opened file: %s\n", _file.name);

    if (snsfs_touch(&fd, "/home/noman.txt") == -1) {
        pprintf("TOUCH FAILED");
    }

    if (snsfs_touch(&fd, "/home/games/noman.txt") == -1) {
        pprintf("TOUCH FAILED");
    }


    // char * data = NULL; // "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. Nullam dictum felis eu pede mollis pretium. Integer tincidunt. Cras dapibus. Vivamus elementum semper nisi. Aenean vulputate eleifend tellus. Aenean leo ligula, porttitor eu, consequat vitae, eleifend ac, enim. Aliquam lorem ante, dapibus in, viverra quis, feugiat a, tellus. Phasellus viverra nulla ut metus varius laoreet. Quisque rutrum. Aenean imperdiet. Etiam ultricies nisi vel augue. Curabitur ullamcorper ultricies nisi. Nam eget dui. Etiam rhoncus. Maecenas tempus, tellus eget condimentum rhoncus, sem quam semper libero, sit amet adipiscing sem neque sed ipsum. Nam quam nunc, blandit vel, luctus pulvinar, hendrerit id, lorem. Maecenas nec odio et ante tincidunt tempus. Donec vitae sapien ut libero venenatis faucibus. Nullam quis ante. Etiam sit amet orci eget eros faucibus tincidunt. Duis leo. Sed fringilla mauris sit amet nibh. Donec sodales sagittis magna. Sed consequat, leo eget bibendum sodales, augue velit cursus nunc, quis gravida magna mi a libero. Fusce vulputate eleifend sapien. Vestibulum purus quam, scelerisque ut, mollis sed, nonummy id, metus. Nullam accumsan lorem in dui. Cras ultricies mi eu turpis hendrerit fringilla. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; In ac dui quis mi consectetuer lacinia. Nam pretium turpis et arcu. Duis arcu tortor, suscipit eget, imperdiet nec, imperdiet iaculis, ipsum. Sed aliquam ultrices mauris. Integer ante arcu, accumsan a, consectetuer eget, posuere ut, mauris. Praesent adipiscing. Phasellus ullamcorper ipsum rutrum nunc. Nunc nonummy metus. Vestibulum volutpat pretium libero. Cras id dui. Aenean ut eros et nisl sagittis vestibulum. Nullam nulla eros, ultricies sit amet, nonummy id, imperdiet feugiat, pede. Sed lectus. Donec mollis hendrerit risus. Phasellus nec sem in justo pellentesque facilisis. Etiam imperdiet imperdiet orci. Nunc nec neque. Phasellus leo dolor, tempus non, auctor et, hendrerit quis, nisi. Curabitur ligula sapien, tincidunt non, euismod vitae, posuere imperdiet, leo. Maecenas malesuada. Praesent congue erat at massa. Sed cursus turpis vitae tortor. Donec posuere vulputate arcu. Phasellus accumsan cursus velit. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Sed aliquam, nisi quis porttitor congue, elit erat euismod orci, ac placerat dolor lectus quis orci. Phasellus consectetuer vestibulum elit. Aenean tellus metus, bibendum sed, posuere ac, mattis non, nunc. Vestibulum fringilla pede sit amet augue. In turpis. Pellentesque posuere. Praesent turpis. Aenean posuere, tortor sed cursus feugiat, nunc augue blandit nunc, eu sollicitudin urna dolor sagittis lacus. Donec elit libero, sodales nec, volutpat a, suscipit non, turpis. Nullam sagittis. Suspendisse pulvinar, augue ac venenatis condimentum, sem libero volutpat nibh, nec pellentesque velit pede quis nunc. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Fusce id purus. Ut varius tincidunt libero. Phasellus dolor. Maecenas vestibulum mollis diam. Pellentesque ut neque. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. In dui magna, posuere eget, vestibulum et, tempor auctor, justo. In ac felis quis tortor malesuada pretium. Pellentesque auctor neque nec urna. Proin sapien ipsum, porta a, auctor quis, euismod ut, mi. Aenean viverra rhoncus pede. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Ut non enim eleifend felis pretium feugiat. Vivamus quis mi. Phasellus a est. Phasellus magna. In hac habitasse platea dictumst. Curabitur at lacus ac velit ornare lobortis. Cur";
    // int data_size = 0; // BLOCK_SIZE;
    // if (snsfs_cat(&fd, "/home/games/a.txt", data, data_size) == -1) {
    //     pprintf("CAT FAILED");
    // }

    snsfs_touch(&fd, "/shaheer.txt");
    snsfs_mkdir(&fd, "/games");

    if (snsfs_ls(&fd, "/") == -1) {
        pprintf("LS FAILED");
    }

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
            .inode_index = (0 + 1), // +1 because we write fake inode_index
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

    int offset = ((*inode_index_count) * sizeof(char) * 4);
    while (strncmp(block_buffer + offset, "\0\0\0\0", sizeof(char) * 4) != 0 && (*inode_index_count < MAX_CHILD_INODE_COUNT)) {
        memcpy((inodes_in_dir + (*inode_index_count)), block_buffer + offset, sizeof(char) * 4);
        (*inode_index_count)++;
        offset = ((*inode_index_count) * sizeof(char) * 4);
    }

    return 1;
}

int add_inode_to_dir(int * fd, inode * parent_inode, int parent_inode_index, int inode_index) {
    if (fd == NULL || *fd < 0 || parent_inode == NULL) {
        return -1;
    }

    if (inode_index < 0 || inode_index >= TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid inode index provided [add_inode_to_dir]");
        return -1;
    }

    char * block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE);
    if (block_buffer == NULL) {
        return -1;
    }

    // fill block with '\0'
    memset(block_buffer, '\0', sizeof(char) * BLOCK_SIZE);

    int data_block_index = parent_inode->pointers[0];
    // if parent inode has no children, then create an empty data block
    if (data_block_index == 0) {
        if (write_data(fd, block_buffer, sizeof(char) * BLOCK_SIZE, parent_inode_index) == -1) {
            free(block_buffer);
            return -1;
        }

        // reread after write
        if (read_inode(fd, parent_inode, parent_inode_index) == -1) {
            free(block_buffer);
            return -1;
        }

        data_block_index = parent_inode->pointers[0];
    }

    if (read_block(fd, (void *) block_buffer, data_block_index) == -1) {
        free(block_buffer);
        return -1;
    }

    int32_t inode_children[MAX_CHILD_INODE_COUNT];
    int child_count = 0;

    if (load_inodes_indexes_to_array(block_buffer, inode_children, &child_count) == -1) {
        free(block_buffer);
        return -1;
    }

    // inode_children[child_count] = ((int32_t) inode_index);

    // add to block_buffer
    memcpy(block_buffer + (child_count * sizeof(char) * 4), &inode_index, sizeof(char) * 4);

    // increment node size
    parent_inode->size += 4;
    if (write_inode(fd, parent_inode, parent_inode_index) == -1) {
        free(block_buffer);
        return -1;
    }

    if (write_block(fd, (void *) block_buffer, data_block_index) == -1) {
        free(block_buffer);
        return -1;
    }

    free(block_buffer);
    return 1;
}

int snsfs_mknod(int * fd, char * inode_name, int inode_name_len, int type) {
    if (fd == NULL || *fd < 1 || inode_name == NULL) {
        pprintf("Invalid parameters provided [snsfs_mknod]");
        return -1;
    }

    if (strlen(inode_name) < 0 || strlen(inode_name) > MAX_FILENAME_LENGTH) {
        pprintf("Invalid inode name length [snsfs_mknod]");
        return -1;
    }

    if (type < IS_EMPTY_INODE || type > IS_FILE_INODE) {
        pprintf("Invalid inode type provided [snsfs_mknod]");
        return -1;
    }

    inode inode_buff = {
        .size = 0,
        .type = ((u_int32_t) type),
        .uid = 0,
        .gid = 0,
        .last_accessed = 0,
        .last_changed = 0,
        .created_at = ((u_int32_t) time(NULL)),
    };

    memset(inode_buff.pointers, 0, (NO_OF_DIRECT_INDEXES + NO_OF_INDIRECT_INDEXES) * sizeof(int32_t));

    int inode_index = add_inode(fd, &inode_buff);
    if (inode_index == -1) {
        pprintf("Unable to create inode [snsfs_mknod]");
        return -1;
    }

    if (add_dentry(fd, inode_name, inode_name_len, inode_index) == -1) {
        pprintf("Unable to create dentry [snsfs_mknod]");
        return -1;
    }

    return inode_index;
}

int is_child_inode(int * fd, int child_inode_index, int * child_inodes, int child_inodes_count, int inode_type) {
    if (child_inode_index < 0 || child_inode_index > TOTAL_NO_OF_INODES(SIZEOF_INODE)) {
        pprintf("Invalid child inode index provided [is_child_inode]");
        return 0;
    }

    if (fd == NULL || *fd == -1 || child_inodes == NULL || child_inodes_count <= 0) {
        pprintf("Invalid parameters provided [is_child_inode]");
        return 0;
    }

    int i;
    inode inode_buff;

    for (i = 0; i < child_inodes_count; i++) {
        if (child_inodes[i] == child_inode_index) {
            if (read_inode(fd, &inode_buff, child_inode_index) == -1) {
                continue;
            }

            // child inode should be a directory
            if (inode_buff.type == inode_type) {
                return 1;
            }
        }
    }

    return 0;
}

int append_child_inode_to_parent(int * fd, inode * inode_buff, int parent_inode_index, int child_inodes_count, int child_inode_index) {
    if (fd == NULL || *fd == -1 || inode_buff == NULL) {
        // invalid params
        return -1;
    }

    if (child_inodes_count > MAX_CHILD_INODE_COUNT) {
        // can't add more child nodes
        return -1;
    }

    int total_no_of_inodes = TOTAL_NO_OF_INODES(SIZEOF_INODE);
    if (parent_inode_index < 0 || child_inode_index < 0 || parent_inode_index >= total_no_of_inodes || child_inode_index >= total_no_of_inodes) {
        // invalid parent or child inode index
        return -1;
    }

    int block_buffer_size = (sizeof(char) * BLOCK_SIZE);
    char * block_buffer = (char *) malloc(block_buffer_size);
    if (block_buffer == NULL) {
        // unable to alloc mem
        return -1;
    }

    // fill block buffer with null-terminators
    memset(block_buffer, '\0', block_buffer_size);

    // parent inode doesnt have any children
    if (inode_buff->pointers[0] == 0) {
        if (write_data(fd, block_buffer, block_buffer_size, parent_inode_index) == -1) {
            // unable to write data
            free(block_buffer);
            return -1;
        }

        // update inode after adding new data block
        if (read_inode(fd, inode_buff, parent_inode_index) == -1) {
            // unable to read inode
            free(block_buffer);
            return -1;
        }
    }

    if (read_block(fd, (void *) block_buffer, inode_buff->pointers[0]) == -1) {
        // unable to read block
        free(block_buffer);
        return -1;
    }

    int32_t child_inode_index_32b = ((int32_t) child_inode_index);

    // append 4 bytes of our child inodex index to our block buffer
    memcpy(block_buffer + (child_inodes_count * sizeof(char) * 4), &child_inode_index_32b, (sizeof(char) * 4));

    if (write_block(fd, (void *) block_buffer, inode_buff->pointers[0]) == -1) {
        // unable to write block
        free(block_buffer);
        return -1;
    }

    free(block_buffer);
    return 1;
}

int load_child_inodes(int * fd, inode * inode_buff, int32_t * child_inodes, int * child_inodes_count) {
    if (fd == NULL || *fd == -1 || inode_buff == NULL || child_inodes == NULL) {
        // invalid params
        return -1;
    }

    if ((*child_inodes_count) > MAX_CHILD_INODE_COUNT) {
        // can't add more child nodes
        return -1;
    }

    if (inode_buff->type != IS_DIR_INODE) {
        // cant load non-dir type inodes' childs
        return -1;
    }

    int block_buffer_size = (sizeof(char) * BLOCK_SIZE);
    char * block_buffer = (char *) malloc(block_buffer_size);
    if (block_buffer == NULL) {
        // unable to alloc mem
        return -1;
    }

    // reset child inodes count
    *child_inodes_count = 0;

    // reset child inodes
    memset(child_inodes, ((int32_t) 0), sizeof(int32_t));

    // parent has no child inodes
    if (inode_buff->pointers[0] == 0) {
        free(block_buffer);
        return 1;
    }

    if (read_block(fd, (void *) block_buffer, inode_buff->pointers[0]) == -1) {
        // unable to read block
        free(block_buffer);
        return -1;
    }

    int offset = 0;
    char * null_str = "\0\0\0\0";
    while (strncmp((block_buffer + offset), null_str, (sizeof(char) * 4)) != 0) {
        offset += (sizeof(char) * 4);
        (*child_inodes_count)++;
    }

    // idk if this works or not | TODO: REMOVE THIS AFTER TESTING
    if ((*child_inodes_count) > 0) {
        memcpy(child_inodes, block_buffer, (sizeof(int32_t) * (*child_inodes_count)));
    }

    return 1;
}

int snsfs_mkdir(int * fd, char * path) {
    if (fd == NULL || *fd == -1 || path == NULL) {
        pprintf("Invalid parameters provided [snsfs_mkdir]");
        return -1;
    }

    char * aux_path = (char *) malloc(sizeof(char) * strlen(path) + 1); // +1 for null-terminator
    if (aux_path == NULL) {
        pprintf("Unable to allocate memory [snsfs_mkdir]");
        return -1;
    }

    memset(aux_path, '\0', ((int) strlen(path) + 1));
    strncpy(aux_path, path, strlen(path));

    char * inode_name = NULL;
    inode inode_buff;
    int parent_inode_index = 0, child_inode_index = 0, lookup_retries = 0, child_inodes_count = 0;
    int inode_name_len = 0;
    int exhaust_strsep = 0, new_dir = 0;
    int32_t child_inodes[MAX_CHILD_INODE_COUNT];

    while ((inode_name = strsep(&aux_path, "/")) != NULL) {
        if (exhaust_strsep == 1) {
            continue;
        }

        inode_name_len = strlen(inode_name);
        if (inode_name_len < 0 || inode_name_len > MAX_FILENAME_LENGTH) {
            pprintf("Provided directory name exceeds the maximum filename length (32 chars) [snsfs_mkdir]");
            exhaust_strsep = 1;
            continue;
        }

        if (strcmp(inode_name, "") == 0) {
            // inode is root
            child_inode_index = 0;
        } else {
            // is not root
            lookup_retries = 0;

            if (child_inodes_count > 0) {
                do {
                    child_inode_index = (dentry_lookup(fd, inode_name, lookup_retries) - 1);
                    lookup_retries++;
                } while ((!(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, IS_DIR_INODE)) && (lookup_retries < child_inodes_count)));
            }

            // is_child_inode should not only act as an includes, but also check
            // whether the child inode founded is a directory or not
            // if not directory, then return 0

            if ((child_inodes_count <= 0) || !(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, IS_DIR_INODE))) {
                // is not a child inode currently
                // make a new node, and append to the parent's child inodes
                child_inode_index = snsfs_mknod(fd, inode_name, inode_name_len, IS_DIR_INODE);
                if (child_inode_index == -1) {
                    pprintf("Failed to create a child inode [snsfs_mkdir]");
                    exhaust_strsep = 1;
                    continue;
                }

                if (append_child_inode_to_parent(fd, &inode_buff, parent_inode_index, child_inodes_count, child_inode_index) == -1) {
                    pprintf("Failed to append child inode to parent [snsfs_mkdir]");
                    exhaust_strsep = 1;
                    continue;
                }

                new_dir = 1;
            }

            // should be child node by now
        }

        // our old child node is the new parent node
        parent_inode_index = child_inode_index;
        if (read_inode(fd, &inode_buff, parent_inode_index) == -1) {
            pprintf("Unable to read inode [snsfs_mkdir]");
            exhaust_strsep = 1;
            continue;
        }

        // read the parent's data block, updates the child_inodes array and child_inodes_count
        if (load_child_inodes(fd, &inode_buff, child_inodes, &child_inodes_count) == -1) {
            pprintf("Unable to load child inodes [snsfs_mkdir]");
            exhaust_strsep = 1;
            continue;
        }
    }

    if (exhaust_strsep == 0 && new_dir == 1) {
        printf("[SUCCESS] directory created {%s}\n", path);
    }

    return (exhaust_strsep == 0 ? 1 : -1);
}

int snsfs_cd(int * fd, char * path) {
    if (fd == NULL || *fd == -1 || path == NULL) {
        pprintf("Invalid parameters provided [snsfs_cd]");
        return -1;
    }

    char * aux_path = (char *) malloc(sizeof(char) * strlen(path) + 1); // +1 for null-terminator
    if (aux_path == NULL) {
        pprintf("Unable to allocate memory [snsfs_cd]");
        return -1;
    }

    memset(aux_path, '\0', ((int) strlen(path) + 1));
    strncpy(aux_path, path, strlen(path));

    char * inode_name = NULL;
    char * inode_name_buff = NULL;
    inode inode_buff;
    int parent_inode_index = 0, child_inode_index = 0, lookup_retries = 0, child_inodes_count = 0;
    int inode_name_len = 0;
    int exhaust_strsep = 0, dir_exists = 1;
    int32_t child_inodes[MAX_CHILD_INODE_COUNT];

    while ((inode_name = strsep(&aux_path, "/")) != NULL) {
        if (exhaust_strsep == 1) {
            continue;
        }

        inode_name_len = strlen(inode_name);
        if (inode_name_len < 0 || inode_name_len > MAX_FILENAME_LENGTH) {
            pprintf("Provided directory name exceeds the maximum filename length (32 chars) [snsfs_cd]");
            exhaust_strsep = 1;
            continue;
        }

        if (strcmp(inode_name, "") == 0) {
            // inode is root
            child_inode_index = 0;
        } else {
            // is not root
            lookup_retries = 0;

            if (child_inodes_count > 0) {
                do {
                    child_inode_index = (dentry_lookup(fd, inode_name, lookup_retries) - 1);
                    lookup_retries++;
                } while ((!(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, IS_DIR_INODE)) && (lookup_retries < child_inodes_count)));
            }

            // is_child_inode should not only act as an includes, but also check
            // whether the child inode founded is a directory or not
            // if not directory, then return 0

            if ((child_inodes_count <= 0) || !(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, IS_DIR_INODE))) {
                // is not a child inode currently
                pprintf("Failed to find child inode [snsfs_cd]");
                dir_exists = 0;
                exhaust_strsep = 1;
                continue;
            }

            // should be child node by now
            if (inode_name_buff == NULL) {
                inode_name_buff = (char *) malloc(sizeof(char) * inode_name_len + 1);

                memset(inode_name_buff, '\0', sizeof(char) * inode_name_len + 1);
                memcpy(inode_name_buff, inode_name, sizeof(char) * inode_name_len);

                memcpy(_file.name, inode_name_buff, sizeof(char) * inode_name_len + 1);

                free(inode_name_buff);
                inode_name_buff = NULL; // just in case
            }
        }

        // our old child node is the new parent node
        parent_inode_index = child_inode_index;
        if (read_inode(fd, &inode_buff, parent_inode_index) == -1) {
            pprintf("Unable to read inode [snsfs_cd]");
            exhaust_strsep = 1;
            continue;
        }

        // read the parent's data block, updates the child_inodes array and child_inodes_count
        if (load_child_inodes(fd, &inode_buff, child_inodes, &child_inodes_count) == -1) {
            pprintf("Unable to load child inodes [snsfs_cd]");
            exhaust_strsep = 1;
            continue;
        }
    }

    if (exhaust_strsep == 0 && dir_exists == 1) {
        strcpy(_file.path, path);
        printf("[SUCCESS] directory changed {%s}\n", path);
    }

    return (exhaust_strsep == 0 ? 1 : -1);
}

int snsfs_touch(int * fd, char * path) {
    if (fd == NULL || *fd == -1 || path == NULL) {
        pprintf("Invalid parameters provided [snsfs_touch]");
        return -1;
    }

    char * aux_path = (char *) malloc(sizeof(char) * strlen(path) + 1); // +1 for null-terminator
    if (aux_path == NULL) {
        pprintf("Unable to allocate memory [snsfs_touch]");
        return -1;
    }

    memset(aux_path, '\0', ((int) strlen(path) + 1));
    strncpy(aux_path, path, strlen(path));

    int delim_count = 1; // starting from 1, since root is compulsory
    int i = 0;

    // counts num of delimiter ('/')
    while (aux_path[i] != '\0') {
        if (aux_path[i] == '/') {
            delim_count++;
        }

        i++;
    }

    // reset for use in delimiter count comparison
    i = 0;

    char * inode_name = NULL;
    inode inode_buff;
    int parent_inode_index = 0, child_inode_index = 0, lookup_retries = 0, child_inodes_count = 0;
    int inode_name_len = 0, inode_type = IS_DIR_INODE;
    int exhaust_strsep = 0, new_file = 0;
    int32_t child_inodes[MAX_CHILD_INODE_COUNT];

    while ((inode_name = strsep(&aux_path, "/")) != NULL) {
        if (exhaust_strsep == 1) {
            continue;
        }

        inode_name_len = strlen(inode_name);
        if (inode_name_len < 0 || inode_name_len > MAX_FILENAME_LENGTH) {
            pprintf("Provided directory name exceeds the maximum filename length (32 chars) [snsfs_touch]");
            exhaust_strsep = 1;
            continue;
        }

        // increase depth count
        i++;
        if (i == delim_count) {
            inode_type = IS_FILE_INODE;
        }

        if (strcmp(inode_name, "") == 0) {
            // inode is root
            child_inode_index = 0;
        } else {
            // is not root
            lookup_retries = 0;

            if (child_inodes_count > 0) {
                do {
                    child_inode_index = (dentry_lookup(fd, inode_name, lookup_retries) - 1);
                    lookup_retries++;
                } while ((!(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, inode_type)) && (lookup_retries < child_inodes_count)));
            }

            // is_child_inode should not only act as an includes, but also check
            // whether the child inode founded is a directory or not
            // if not directory, then return 0

            if ((child_inodes_count <= 0) || !(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, inode_type))) {
                // is not a child inode currently
                // if it is a directory, then just exit
                if (inode_type == IS_DIR_INODE) {
                    pprintf("Invalid directory lookup [snsfs_touch]");
                    exhaust_strsep = 1;
                    continue;
                }

                // make a new node, and append to the parent's child inodes
                child_inode_index = snsfs_mknod(fd, inode_name, inode_name_len, IS_FILE_INODE);
                if (child_inode_index == -1) {
                    pprintf("Failed to create a child inode [snsfs_touch]");
                    exhaust_strsep = 1;
                    continue;
                }

                if (append_child_inode_to_parent(fd, &inode_buff, parent_inode_index, child_inodes_count, child_inode_index) == -1) {
                    pprintf("Failed to append child inode to parent [snsfs_touch]");
                    exhaust_strsep = 1;
                    continue;
                }

                new_file = 1;
            }

            // should be child node by now
        }

        // our old child node is the new parent node
        parent_inode_index = child_inode_index;
        if (read_inode(fd, &inode_buff, parent_inode_index) == -1) {
            pprintf("Unable to read inode [snsfs_touch]");
            exhaust_strsep = 1;
            continue;
        }

        // read the parent's data block, updates the child_inodes array and child_inodes_count
        if (inode_type == IS_DIR_INODE) {
            if (load_child_inodes(fd, &inode_buff, child_inodes, &child_inodes_count) == -1) {
                pprintf("Unable to load child inodes [snsfs_touch]");
                exhaust_strsep = 1;
                continue;
            }
        }
    }

    if (exhaust_strsep == 0 && new_file == 1) {
        printf("[SUCCESS] file created {%s}\n", path);
    }

    return (exhaust_strsep == 0 ? 1 : -1);
}

int snsfs_cat(int * fd, char * path, char * data, int data_size) {
    if (fd == NULL || *fd == -1 || path == NULL) {
        pprintf("Invalid parameters provided [snsfs_cat]");
        return -1;
    }

    char * aux_path = (char *) malloc(sizeof(char) * strlen(path) + 1); // +1 for null-terminator
    if (aux_path == NULL) {
        pprintf("Unable to allocate memory [snsfs_cat]");
        return -1;
    }

    memset(aux_path, '\0', ((int) strlen(path) + 1));
    strncpy(aux_path, path, strlen(path));

    int delim_count = 1; // starting from 1, since root is compulsory
    int i = 0;

    // counts num of delimiter ('/')
    while (aux_path[i] != '\0') {
        if (aux_path[i] == '/') {
            delim_count++;
        }

        i++;
    }

    // reset for use in delimiter count comparison
    i = 0;

    char * inode_name = NULL;
    inode inode_buff;
    int parent_inode_index = 0, child_inode_index = 0, lookup_retries = 0, child_inodes_count = 0;
    int inode_name_len = 0, inode_type = IS_DIR_INODE;
    int exhaust_strsep = 0, exited_by_will = 0;
    int32_t child_inodes[MAX_CHILD_INODE_COUNT];
    char prompt = 'y', buff_consumer;
    int block_counter = 0, block_pointer = 0;
    char * block_buffer = NULL;

    while ((inode_name = strsep(&aux_path, "/")) != NULL) {
        if (exhaust_strsep == 1) {
            continue;
        }

        inode_name_len = strlen(inode_name);
        if (inode_name_len < 0 || inode_name_len > MAX_FILENAME_LENGTH) {
            pprintf("Provided directory name exceeds the maximum filename length (32 chars) [snsfs_cat]");
            exhaust_strsep = 1;
            continue;
        }

        // increase depth count
        i++;
        if (i == delim_count) {
            inode_type = IS_FILE_INODE;
        }

        if (strcmp(inode_name, "") == 0) {
            // inode is root
            child_inode_index = 0;
        } else {
            // is not root
            lookup_retries = 0;

            if (child_inodes_count > 0) {
                do {
                    child_inode_index = (dentry_lookup(fd, inode_name, lookup_retries) - 1);
                    lookup_retries++;
                } while ((!(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, inode_type)) && (lookup_retries < child_inodes_count)));
            }

            // is_child_inode should not only act as an includes, but also check
            // whether the child inode founded is a directory or not
            // if not directory, then return 0

            if ((child_inodes_count <= 0) || !(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, inode_type))) {
                // is not a child inode currently
                pprintf("Invalid directory lookup [snsfs_cat]");
                exhaust_strsep = 1;
                continue;
            }
        }

        // our old child node is the new parent node
        parent_inode_index = child_inode_index;
        if (read_inode(fd, &inode_buff, parent_inode_index) == -1) {
            pprintf("Unable to read inode [snsfs_cat]");
            exhaust_strsep = 1;
            continue;
        }

        if (inode_buff.type == IS_FILE_INODE) {
            if (data != NULL) {
                // we have data to write
                if (write_data(fd, data, data_size, parent_inode_index) == -1) {
                    pprintf("Unable to write data [snsfs_cat]");
                    exhaust_strsep = 1;
                    continue;
                }

                pprintf("Data written!");
            } else {
                block_buffer = (char *) malloc(sizeof(char) * BLOCK_SIZE + 1);
                if (block_buffer == NULL) {
                    // failed to read
                    exhaust_strsep = 1;
                    continue;
                }

                memset(block_buffer, '\0', sizeof(char) * BLOCK_SIZE);

                while (prompt == 'y') {
                    if (read_data_by_block(fd, block_buffer, &inode_buff, block_counter) == -1) {
                        free(block_buffer);
                        pprintf("There was a problem while trying to read the block...");
                        exited_by_will = 1;
                        exhaust_strsep = 1;
                        break;
                    }

                    pprintf("-----------------------------------------------------------\n");

                    for (block_pointer = 0; block_buffer[block_pointer] != '\0' && block_pointer < BLOCK_SIZE; block_pointer++) {
                        printf("%c", block_buffer[block_pointer]);
                    }

                    printf("\n\n");
                    pprintf("-----------------------------------------------------------");

                    printf("Read next block? [y/N]: ");
                    scanf("%c", &prompt);
                    scanf("%c", &buff_consumer);
                    printf("\n\n");
                    if (prompt == 'y') {
                        block_counter++;
                    } else {
                        free(block_buffer);
                        exited_by_will = 1;
                        exhaust_strsep = 1;
                        break;
                    }
                }
            }
        }

        // read the parent's data block, updates the child_inodes array and child_inodes_count
        if (inode_type == IS_DIR_INODE) {
            if (load_child_inodes(fd, &inode_buff, child_inodes, &child_inodes_count) == -1) {
                pprintf("Unable to load child inodes [snsfs_cat]");
                exhaust_strsep = 1;
                continue;
            }
        }
    }

    return (exited_by_will == 1 ? 1 : -1);
}

int snsfs_ls(int * fd, char * path) {
    if (fd == NULL || *fd == -1 || path == NULL) {
        pprintf("Invalid parameters provided [snsfs_ls]");
        return -1;
    }

    char * aux_path = (char *) malloc(sizeof(char) * strlen(path) + 1); // +1 for null-terminator
    if (aux_path == NULL) {
        pprintf("Unable to allocate memory [snsfs_ls]");
        return -1;
    }

    memset(aux_path, '\0', ((int) strlen(path) + 1));
    strncpy(aux_path, path, strlen(path));

    char * inode_name = NULL;
    char * inode_name_buff = NULL;
    inode inode_buff;
    int parent_inode_index = 0, child_inode_index = 0, lookup_retries = 0, child_inodes_count = 0;
    int inode_name_len = 0;
    int exhaust_strsep = 0, dir_exists = 1;
    int32_t child_inodes[MAX_CHILD_INODE_COUNT];

    while ((inode_name = strsep(&aux_path, "/")) != NULL) {
        if (exhaust_strsep == 1) {
            continue;
        }

        inode_name_len = strlen(inode_name);
        if (inode_name_len < 0 || inode_name_len > MAX_FILENAME_LENGTH) {
            pprintf("Provided directory name exceeds the maximum filename length (32 chars) [snsfs_ls]");
            exhaust_strsep = 1;
            continue;
        }

        if (strcmp(inode_name, "") == 0) {
            // inode is root
            child_inode_index = 0;
        } else {
            // is not root
            lookup_retries = 0;

            if (child_inodes_count > 0) {
                do {
                    child_inode_index = (dentry_lookup(fd, inode_name, lookup_retries) - 1);
                    lookup_retries++;
                } while ((!(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, IS_DIR_INODE)) && (lookup_retries < child_inodes_count)));
            }

            // is_child_inode should not only act as an includes, but also check
            // whether the child inode founded is a directory or not
            // if not directory, then return 0

            if ((child_inodes_count <= 0) || !(is_child_inode(fd, child_inode_index, child_inodes, child_inodes_count, IS_DIR_INODE))) {
                // is not a child inode currently
                pprintf("Failed to find child inode [snsfs_ls]");
                dir_exists = 0;
                exhaust_strsep = 1;
                continue;
            }

            // should be child node by now
        }

        // our old child node is the new parent node
        parent_inode_index = child_inode_index;
        if (read_inode(fd, &inode_buff, parent_inode_index) == -1) {
            pprintf("Unable to read inode [snsfs_ls]");
            exhaust_strsep = 1;
            continue;
        }

        // read the parent's data block, updates the child_inodes array and child_inodes_count
        if (load_child_inodes(fd, &inode_buff, child_inodes, &child_inodes_count) == -1) {
            pprintf("Unable to load child inodes [snsfs_cd]");
            exhaust_strsep = 1;
            continue;
        }
    }

    int i, dentry_index;
    dentry dentry_buff;

    if (exhaust_strsep == 0 && dir_exists == 1) {
        pprintf("\n[snsfs_ls]\n---------------------------------------------------\n");

        for (i = -1; i < child_inodes_count; i++) {
            dentry_index = dentry_lookup_using_inode_index(fd, (i == -1 ? parent_inode_index : child_inodes[i]));

            // this shouldnt be possible
            if (dentry_index == -1) {
                return -1;
            }

            if (read_dentry(fd, &dentry_buff, dentry_index) == -1) {
                pprintf("Unable to read dentry [snsfs_ls]");
                return -1;
            }

            if (i != -1) {
                if (read_inode(fd, &inode_buff, child_inodes[i]) == -1) {
                    pprintf("Unable to read child inodes [snsfs_ls]");
                    return -1;
                }
            }

            // filename, created_at, size, uid, gid
            printf("%s\t%ld\t%d\t%d\t%d\t[%s]\n", dentry_buff.filename, inode_buff.created_at, inode_buff.size, inode_buff.uid, inode_buff.gid, (inode_buff.type == IS_FILE_INODE ? "FILE" : "DIR"));
            if (i == -1) {
                pprintf("\n---------------------------------------------------\n");
            }
        }

        pprintf("\n---------------------------------------------------\n");
    }

    return (exhaust_strsep == 0 ? 1 : -1);
}
