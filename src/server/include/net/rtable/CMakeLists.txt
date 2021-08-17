add_library(
    rtable
    rtable.c
    rtable.h
    hash.h
)

target_link_libraries(rtable node)