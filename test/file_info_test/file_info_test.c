#define _GNU_SOURCE
#include "../../src/include/file_info/file_info.h"
#include "../cutest-1.5/CuTest.h"

#include <openssl/sha.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static unsigned char* file_checksum(struct file_info *_file_info)
{
    char *data;
    asprintf(&data, "%ld%s", _file_info->file_size, _file_info->file_name);
    SHA256(data, SHA256_DIGEST_LENGTH, _file_info->checksum);
    free(data);

    return _file_info->checksum;
}

void TestFileInfo(CuTest *tc)
{
    struct file_info fileinfo;
    file_info_init(&fileinfo);
    // expected
    char *file_name = "../file_info_test/sample_file.txt";
    size_t file_size = 344;
    unsigned char *checksum = file_checksum(&fileinfo);

    file_info_load(file_name, &fileinfo);
    


    CuAssertStrEquals(tc, file_name, fileinfo.file_name);
    CuAssertIntEquals(tc, file_size, fileinfo.file_size);
    CuAssertPtrEquals(tc, checksum, fileinfo.checksum);

}

CuSuite* FileInfoGetSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestFileInfo);
    return suite;
}