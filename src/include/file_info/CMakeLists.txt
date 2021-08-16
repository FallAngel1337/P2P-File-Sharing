add_library(
    file_info
    file_info.c
    file_info.h
)

target_link_libraries(file_info -lcrypto -lconfig)