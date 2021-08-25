cmake_minimum_required(VERSION 3.16)

project(P2P-Client VERSION 0.1)

add_subdirectory(include/net/conn)
add_subdirectory(include/net/seeder)

add_subdirectory(../include/node node)
add_subdirectory(../include/file_info file_info)

add_executable(P2P-Client p2p-client.c)

target_link_libraries(P2P-Client PUBLIC 
                      file_info 
                      node
                      connection
                      seeder
                      -lcrypto -lcjson -lconfig
)