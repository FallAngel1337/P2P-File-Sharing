add_library(
    hash_colision_test
    hash_colision_test.c
    ../../src/server/include/net/rtable/rtable.c
    ../../src/server/include/net/rtable/rtable.h
    ../cutest/CuTest.c
    ../cutest/CuTest.h
)

target_link_libraries(hash_colision_test -lm)