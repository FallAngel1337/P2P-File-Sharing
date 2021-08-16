/**
 * rtable.h - Server's routing table implementation
 * Author: FallAngel
 * Date: 2021-08-16
 * 
 * Copyright (c) 2021
 */

#ifndef _RTABLE_H
#define _RTABLE_H

#include "../../../../include/node/node.h"
#include "../../../../include/json_serialization/json_serialization.h"
#include "hash.h"

struct rtable {
    struct Node *node;
    struct rtable *next;
};

void show_table(struct rtable *_table);
int table_init(struct rtable *_table, size_t _size);
int table_insert(struct rtable *_table, struct Node *node, size_t _size);
int table_remove(struct rtable *_table, struct Node *node, size_t _size);

#endif /* _RTABLE_H */