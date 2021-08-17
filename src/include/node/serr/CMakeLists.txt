add_library(
    node_serr
    node_serr.c
    node_serr.h
)

target_link_libraries(node_serr -lcjson)