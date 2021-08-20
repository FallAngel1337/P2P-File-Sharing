/**
Compiler with: clang serr_fuzzing.c -fsanitize=fuzzer,address ../../../src/include/node/node.c ../../../src/include/node/serr/node_serr.c ../../../src/include/file_info/file_info.c -lcjson -lcrypto
*/
#include <check.h>
#include "../../../src/include/node/serr/node_serr.h"

#ifdef __cplusplus
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    DoSomethingInterestingWithMyAPI(Data, Size);
    return 0;  // Non-zero return values are reserved for future use.
}
#else
int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    struct Node *node = node_create("127.0.0.1", 9999);
    file_info_load((const char*)Data, node->fileinfo);
    node_destroy(node);
    return 0;  // Non-zero return values are reserved for future use.
}
#endif