add_library(
    rtable
    rtable.c
    rtable.h
    hash.h
)

target_include_directories(rtable PUBLIC ../../../../include/ ../../../include/)
target_link_libraries(rtable node)