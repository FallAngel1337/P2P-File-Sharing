#include "rtable.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rtable {
    struct Node *node;
    struct rtable *next;
};

void show_table(struct rtable *_table)
{

}

int table_init(struct rtable *_table, size_t _size)
{
    if (!_table) return -1;

    for (size_t i=0; i < _size; i++) {
        _table[i].node = NULL;
        _table[i].next = NULL;
    }
    return 0;
}

static inline uint64_t get_hash(unsigned char *_hash) 
{
    uint64_t hash = 0;
    for (int i=0; i < 32; i++) hash |= _hash[i];
    return hash;
}

int table_insert(struct rtable *_table, struct Node *node, size_t _size)
{   
    if (!_table || !node || _size <= 0) return -1;

    uint64_t hash = get_hash(node->fileinfo->checksum);
    printf(">> %lu\n", hash);
    struct Node *_node = _table[mulhashing(hash, _size)].node;
    
#ifdef _DEBUG
        printf("hash=%lu\n", hash);
#endif

    if (!_node) {

#ifdef _DEBUG
        printf("table[%lu] is free ...\n", mulhashing(hash, _size));
#endif
        _table[mulhashing(hash, _size)].node = node;

    } else {

#ifdef _DEBUG
        printf("table[%lu] is not free ...\n", mulhashing(hash, _size));
        printf("chaining ...\n");
#endif
        _table[mulhashing(hash, _size)].next = _table;

    }

    return 0;
}

int table_remove(struct rtable *_table, struct Node *node, size_t _size)
{
    uint64_t hash = get_hash(node->fileinfo->checksum);
    if (_table[mulhashing(hash, _size)].next) {
        fprintf(stderr, "This entry(%lu) is chained ... can not delete it due possible miss deletion!\n", mulhashing(hash, _size));
        return -1;
    } else {
        node_destroy(_table[mulhashing(hash, _size)].node);
    }
    return 0;
}