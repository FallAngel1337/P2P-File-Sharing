cmake_minimum_required(VERSION 3.16)

project(Tests VERSION 0.1)

add_library(
    file_info_test
    file_info_test/file_info_test.c
    cutest-1.5/CuTest.h
    cutest-1.5/CuTest.c
    ../src/include/file_info/file_info.c
    ../src/include/file_info/file_info.h
)

add_executable(Tests file_info_test/AllTests.c)

target_link_libraries(Tests PUBLIC file_info_test -lcrypto)