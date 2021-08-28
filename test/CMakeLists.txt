cmake_minimum_required(VERSION 3.10)

project(UnitTests VERSION 1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(serialization)

add_executable(UnitTests AllTests.c)

target_link_libraries(UnitTests PUBLIC
                     serialization
                     -lcrypto -lcjson)