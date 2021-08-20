cmake_minimum_required(VERSION 3.16)

PROJECT(Serialization-Test)

add_subdirectory(../../src/include/node/ node)
add_subdirectory(../../src/include/file_info file_info)

add_executable(Serialization-Test serr_test.c)

target_link_libraries(Serialization-Test PUBLIC 
                      node file_info
                      -lcheck -lm -lsubunit -lrt -pthread)