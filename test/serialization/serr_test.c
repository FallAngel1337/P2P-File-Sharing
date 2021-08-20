#include <check.h>
#include "../../src/include/node/serr/node_serr.h"

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h> // inet_ntoa

START_TEST(serr_test)
{
    // expected
    struct Node *node = node_create("127.0.0.1", 9999);
    file_info_load("../../sample_file.txt", node->fileinfo);
    
    char *json;
    
    json = nodeSerialize(node, SERR_NET);
    printf(">> %s\n", json);
    
    ck_assert_int_eq(node->addr.sin_port, htons(9999));
    ck_assert_ptr_ne(json, NULL);
    ck_assert_str_eq(inet_ntoa(node->addr.sin_addr), "127.0.0.1");

    free(json);

    json = nodeSerialize(node, 0);
    printf("--> %s\n", json);

    struct Node *node2 = NULL;
    node2 = nodeDeserialize(json, node2, 0);
    file_info_load("../sample_file.txt", node2->fileinfo);

    ck_assert_ptr_ne(json, NULL);
    ck_assert_int_ne(node2->addr.sin_port, 9999);
    ck_assert_str_ne(inet_ntoa(node2->addr.sin_addr), "127.0.0.1");
    
    node_destroy(node);
    node_destroy(node2);
}
END_TEST

Suite* serr_suite(void)
{
    Suite *s = NULL;
    TCase *tc_core = NULL;

    s = suite_create("Serialization");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, serr_test);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number = 0;
    Suite *s = NULL;
    SRunner *sr = NULL;

    s = serr_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number = srunner_ntests_run(sr);
    srunner_free(sr);
    return (number == 0) ? 0 : -1;
}