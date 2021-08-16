cmake_minimum_required(VERSION 3.16)

project(P2P-Server VERSION 0.1)

add_subdirectory(include/net/conn)
add_subdirectory(include/net/rtable)

add_subdirectory(../include/node node)
add_subdirectory(../include/logging logs)
add_subdirectory(../include/file_info file_info)
add_subdirectory(../include/json_serialization json_serialization)

add_executable(P2P-Server p2p-server.c)

target_link_libraries(P2P-Server PUBLIC 
                      file_info 
                      json_serialization
                      node
                      logs
                      connection 
                      rtable
                      -lm -lconfig)