/**
 * rtable.h - Server's routing table implementation
 * Author: FallAngel
 * Date: 2021-08-16
 * 
 * Copyright (c) 2021
 */

#ifndef _RTABLE_H
#define _RTABLE_H

#include "../../../../include/file_info/file_info.h"
#include "../../../../include/json_serialization/json_serialization.h"
#include "hash.h"

#define TABLE_MAX 10

struct rtable {
    struct file_info *_file_info;
    struct rtable *next;
};

void show_table(struct rtable *_table);

#endif /* _RTABLE_H */