cmake_minimum_required(VERSION 3.16)

project(P2P-Client VERSION 0.1)

add_subdirectory(include)
add_subdirectory(../include/file_info file_info)
add_subdirectory(../include/json_serialization json_serialization)

add_executable(P2P-Client p2p-client.c)

target_link_libraries(P2P-Client PUBLIC file_info json_serialization -lcrypto -lcjson -lconfig)