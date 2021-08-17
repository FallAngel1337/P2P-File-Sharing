add_library(
    connection
    conn.c
    conn.h
)

target_link_libraries(connection json_serialization)