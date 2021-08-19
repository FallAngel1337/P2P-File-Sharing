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
#include "../../../../include/node/serr/node_serr.h"

struct rtable;

void show_table(struct rtable **_table, size_t rtableSize);

struct rtable** table_create(size_t rtableSize);
void table_destroy(struct rtable **_table, size_t rtableSize);

int table_insert(struct rtable **_table, struct Node *node, size_t rtableSize);
int table_remove(struct rtable **_table, struct Node *node, size_t rtableSize);

struct Node* table_lookup(struct rtable **_table, struct file_info *_file, size_t rtableSize);

#endif /* _RTABLE_H */