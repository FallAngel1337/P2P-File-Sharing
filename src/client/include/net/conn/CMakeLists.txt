add_library(
    connection
    conn.c
    conn.h
)

target_include_directories(connection PUBLIC ../../../../include/ ../../../include/)