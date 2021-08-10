#include "../cutest/CuTest.h"
#include "../../src/include/file_info/file_info.h"

#include <stdio.h>
#include <stdlib.h>

void TestFileInfoLoad(CuTest *tc)
{
    printf("Testing File Info ...\n");
    struct file_info fileinfo;
    const char *sample_file = "../sample_file.txt";
    // expected
    const char *file_name = "sample_file.txt";
    const size_t size = 72;
    const char *checksum = "3ad8caa1f19e7950ab75dc1f123555553a79eafce2f344f7984f1202ae0e5f39";

    // Testing
    file_info_init(&fileinfo);
    file_info_load(sample_file, &fileinfo);

    CuAssertStrEquals(tc, file_name, fileinfo.file_name);
    CuAssertIntEquals(tc, size, fileinfo.file_size);
    CuAssertStrEquals(tc, checksum, fileinfo.checksum);
}

CuSuite* FileInfoLoadGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestFileInfoLoad);
    return suite;
}