add_library(
    connection
    conn.c
    conn.h
)

target_include_directories(connection PUBLIC ../../../../include/ ../../../include/)
target_link_libraries(connection node_serr)
