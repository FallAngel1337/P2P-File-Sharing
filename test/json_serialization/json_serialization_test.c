#include "../cutest/CuTest.h"
#include "../../src/include/file_info/file_info.h"
#include "../../src/include/json_serialization/json_serialization.h"

#include <stdio.h>
#include <stdlib.h>

void TestJsonSerialization(CuTest *tc)
{
    printf("Testing Json Serialization ...\n");
    struct file_info fileinfo;
    const char *sample_file = "../sample_file.txt";
    // expected
    char *file_name;

    file_info_init(&fileinfo);
    file_info_load(sample_file, &fileinfo);

    jsonWriteFile(&file_name, &fileinfo);

    CuAssertStrEquals(tc, file_name, "sample_file.torrent");
    free(file_name);
}

void TestJsonDeserialization(CuTest *tc)
{
    printf("Testing Json Deserialization ...\n");
    struct file_info fileinfo;
    file_info_init(&fileinfo);
    const char *torrent_file = "sample_file.torrent";
    // expected
    const char *file_name = "sample_file.txt";
    size_t file_size = 72;
    const char *checksum = "3ad8caa1f19e7950ab75dc1f123555553a79eafce2f344f7984f1202ae0e5f39";


    char *free_later = jsonReadFile("sample_file.torrent", &fileinfo);

    CuAssertStrEquals(tc, file_name, fileinfo.file_name);
    CuAssertIntEquals(tc, file_size, fileinfo.file_size);
    CuAssertStrEquals(tc, checksum, fileinfo.checksum);

    free(free_later);
}

CuSuite* GetJsonSerializationGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestJsonSerialization);
    SUITE_ADD_TEST(suite, TestJsonDeserialization);
    return suite;
}