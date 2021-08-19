#define _DEBUG

#include "rtable.h"
#include "hash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void show_table(struct rtable **_table, size_t rtableSize)
{
    size_t i;
    for (i=0; i < rtableSize; i++) {
        struct rtable *curr = _table[i];
        printf("================ Entry' %lu ================\n", i);
        while (curr != NULL) {
            if (curr->node) {
                printf("Node IP: %s\n", inet_ntoa(curr->node->addr.sin_addr));
                printf("Node PORT: %d\n", curr->node->addr.sin_port);
                printf("Node filename: %s\n", curr->node->fileinfo->file_name);
                printf("Node file size: %lu\n", curr->node->fileinfo->file_size);  
                printf("Node checksum: %s\n", curr->node->fileinfo->checksum);
                printf("===========================================\n");
            }
            curr = curr->next;
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

static bool check_attribs(struct file_info *_file1, struct file_info *_file2)
{
    if (strcmp(_file1->file_name,  _file2->file_name)
        || _file1->file_size != _file2->file_size
        || strcmp(_file1->checksum, _file2->checksum)
        ) 
        {
            return false;
        } 
    
    return true;
}

struct Node* table_lookup(struct rtable **_table, struct file_info *_file, size_t rtableSize)
{
    struct rtable *curr = _table[mulhashing(get_hash(_file->checksum), rtableSize)];
    while (curr->node) {
        if (check_attribs(curr->node->fileinfo, _file)) return curr->node;
        curr = curr->next;
    }

    return NULL;
}

int table_insert(struct rtable **_table, struct Node *node, size_t rtableSize)
{   
    if (!_table || !node || rtableSize <= 0) return -1;

    uint64_t hash = get_hash(node->fileinfo->checksum);
    struct Node *_node = _table[mulhashing(hash, rtableSize)]->node;
    
#ifdef _DEBUG
    printf("hash=%lu\n", hash);
#endif

    if (!_node) {

#ifdef _DEBUG
        printf("table[%lu] is free ...\n", mulhashing(hash, rtableSize));
#endif
        _table[mulhashing(hash, rtableSize)]->node = node;

    } else {

#ifdef _DEBUG
        printf("table[%lu] is not free ...\n", mulhashing(hash, rtableSize));
        printf("chaining ...\n");
#endif

        struct rtable *curr = _table[mulhashing(hash, rtableSize)];
        while (curr->next) curr = curr->next;

        curr->next = add_table(node);

    }

    return 0;
}

int table_remove(struct rtable **_table, struct Node *node, size_t rtableSize)
{
    uint64_t hash = get_hash(node->fileinfo->checksum);
    if (_table[mulhashing(hash, rtableSize)]->next) {
        fprintf(stderr, "This entry(%lu) is chained ... can not delete it due possible miss deletion!\n", mulhashing(hash, rtableSize));
        return -1;
    } else {
        node_destroy(_table[mulhashing(hash, rtableSize)]->node);
    }
    return 0;
}
