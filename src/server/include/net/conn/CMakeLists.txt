add_library(
    connection
    conn.c
    conn.h
)

target_link_libraries(connection node_serr)