#include "../cutest/CuTest.h"
#include "../../src/include/node/serr/node_serr.h"

#include <stdio.h>
#include <stdlib.h>

void TestNodeSerr(CuTest *tc)
{
    printf("Testing Json Serialization ...\n");

    struct Node *a_node = node_create("0.0.0.0", 8823);
    struct Node *b_node = node_create("127.0.0.1", 7777);
    const char *sample_file = "../sample_file.txt";
    // expected
    const char *file_name = "sample_file.txt";
    char *_torrent_file;
    const char *torrent_file = "sample_file.torrent";
    const char *checksum = "3ad8caa1f19e7950ab75dc1f123555553a79eafce2f344f7984f1202ae0e5f39";
    size_t file_size = 72;

    file_info_load(sample_file, a_node->fileinfo);
    jsonWriteFile(&_torrent_file, a_node);

    printf("Testing Json Deserialization ...\n");

    char *free_later = jsonReadFile(torrent_file, b_node);

    CuAssertStrEquals(tc, _torrent_file, torrent_file);
    CuAssertStrEquals(tc, file_name, b_node->fileinfo->file_name);
    CuAssertIntEquals(tc, file_size, b_node->fileinfo->file_size);
    CuAssertStrEquals(tc, checksum, b_node->fileinfo->checksum);

    free(_torrent_file);
    free(free_later);
    node_destroy(a_node);
    node_destroy(b_node);
}

CuSuite* JsonSerializationSuite()
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, TestNodeSerr);
    return suite;
}