add_library(
    seeder
    seeder.c
    seeder.h
)

add_subdirectory(../../../../include/logging logs)

target_include_directories(seeder PUBLIC ../../../../include/ ../../../include/)
target_link_libraries(seeder logs)