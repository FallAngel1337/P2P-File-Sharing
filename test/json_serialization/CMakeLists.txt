add_library(
    json_serialization_test
    json_serialization_test.c
    ../../src/include/json_serialization/json_serialization.c
    ../../src/include/json_serialization/json_serialization.h
    ../cutest/CuTest.c
    ../cutest/CuTest.h
)