add_library(
    json_serialization
    json_serialization.c
    json_serialization.h
)

target_link_libraries(json_serialization -lcjson)