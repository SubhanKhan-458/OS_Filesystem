cmake_minimum_required(VERSION 3.0.0)
project(sns_fs VERSION 0.1.0 LANGUAGES C)

add_library(sns_fs_lib ./lib/blocks.c ./lib/data_blocks.c ./lib/dentry.c ./lib/indirect_nodes.c ./lib/inodes.c ./lib/olt.c)
add_executable(sns_fs ./src/main.c)
target_link_libraries(sns_fs sns_fs_lib -lm)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
