#define _DEBUG

#include "rtable.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

struct rtable {
    struct Node *node;
    struct rtable *next;
};

int idx = 0;

static inline uint64_t get_hash(unsigned char *_hash) 
{
    uint64_t hash = 0;
    for (int i=0; i < 65; i++) hash += _hash[i];
    return hash;
}

void show_table(struct rtable **_table, size_t _size)
{
    size_t i;
    for (i=0; i < _size; i++) {
        struct rtable *curr = _table[i];
        if (curr->node) {
            struct Node *curr_node = curr->node;
            printf("================ Node' %lu ================\n", i);
            printf("Node IP: %s\n", inet_ntoa(curr_node->addr.sin_addr));
            printf("Node PORT: %d\n", curr_node->addr.sin_port);
            printf("Node filename: %s\n", curr_node->fileinfo->file_name);
            printf("Node file size: %lu\n", curr_node->fileinfo->file_size);  
            printf("Node checksum: %s\n", curr_node->fileinfo->checksum);
            printf("===========================================\n\n");

            while (curr->next) {
                struct Node *curr_node = curr->node;
                printf("================ Node' %lu chain ================\n", i);
                printf("Node IP: %s\n", inet_ntoa(curr_node->addr.sin_addr));
                printf("Node PORT: %d\n", curr_node->addr.sin_port);
                printf("Node filename: %s\n", curr_node->fileinfo->file_name);
                printf("Node file size: %lu\n", curr_node->fileinfo->file_size);  
                printf("Node checksum: %s\n", curr_node->fileinfo->checksum);
                printf("=================================================\n\n");
                curr = curr->next;
            }
        }
    }
}

struct rtable** table_create(size_t rtableSize)
{
    struct rtable **table = malloc(sizeof(struct rtable*) * rtableSize);
    
    for (size_t i=0; i < rtableSize; i++) {
        table[i] = malloc(sizeof(struct rtable));
        table[i]->node = NULL;
        table[i]->next = NULL;
    }

    return table;
}

void table_destroy(struct rtable **_table, size_t rtableSize)
{
    for (size_t i = 0; i < rtableSize; i++) free(_table[i]);
}

static struct rtable* add_table(struct Node *node)
{
    struct rtable *new_table = malloc(sizeof(struct rtable));
    new_table->node = node;
    new_table->next = NULL;
    return new_table;
}

int table_insert(struct rtable **_table, struct Node *node, size_t _size)
{   
    if (!_table || !node || _size <= 0) return -1;

    uint64_t hash = get_hash(node->fileinfo->checksum);
    struct Node *_node = _table[mulhashing(hash, _size)]->node;
    
#ifdef _DEBUG
    printf("hash=%lu\n", hash);
#endif

    if (!_node) {

#ifdef _DEBUG
        printf("table[%lu] is free ...\n", mulhashing(hash, _size));
#endif
        _table[mulhashing(hash, _size)]->node = node;

    } else {

#ifdef _DEBUG
        printf("table[%lu] is not free ...\n", mulhashing(hash, _size));
        printf("chaining ...\n");
#endif

        struct rtable *curr = _table[mulhashing(hash, _size)];
        while (curr->next) curr = curr->next;

        curr->next = add_table(node);

    }

    return 0;
}

int table_remove(struct rtable **_table, struct Node *node, size_t _size)
{
    uint64_t hash = get_hash(node->fileinfo->checksum);
    if (_table[mulhashing(hash, _size)]->next) {
        fprintf(stderr, "This entry(%lu) is chained ... can not delete it due possible miss deletion!\n", mulhashing(hash, _size));
        return -1;
    } else {
        node_destroy(_table[mulhashing(hash, _size)]->node);
    }
    return 0;
}
