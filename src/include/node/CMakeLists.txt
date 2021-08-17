add_library(
    node
    node.c
    node.h
)

add_subdirectory(serr/)

target_link_libraries(node file_info node_serr)