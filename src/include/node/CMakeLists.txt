add_library(
    node
    node.c
    node.h
)

target_link_libraries(node file_info)