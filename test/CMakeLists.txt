cmake_minimum_required(VERSION 3.16)

project(UnitTests VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(file_info)
add_subdirectory(json_serialization)

add_executable(UnitTests AllTests.c)

target_link_libraries(UnitTests PUBLIC file_info_test json_serialization_test -lcrypto -lcjson)