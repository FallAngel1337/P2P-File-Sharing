cmake_minimum_required(VERSION 3.10)

project(P2P-Server VERSION 0.1)

add_subdirectory(include/net/conn)
add_subdirectory(include/net/rtable)

add_subdirectory(../include/node node)
add_subdirectory(../include/logging logs)
add_subdirectory(../include/file_info file_info)
add_executable(P2P-Server p2p-server.c)

target_include_directories(P2P-Server PUBLIC ../include/ include/)
target_link_libraries(P2P-Server PUBLIC 
                      file_info 
                      node
                      logs
                      connection 
                      rtable
                      -lm -lconfig)