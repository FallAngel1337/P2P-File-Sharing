/**
 * node.h - Node structure definition
 * Author: FallAngel
 * Date: 2021-08-13
 * 
 * Copyright (c) 2021
 */
/**
 * The node abstraction structure.
 * And some other related functions.
*/

#ifndef _NODE_H
#define _NODE_H

#include "../file_info/file_info.h"

#include <stdint.h>
#include <netinet/in.h> // struct sockaddr_in

struct Node {
    struct file_info *fileinfo;
    struct sockaddr_in addr;
};

struct Node* node_create(const char *__restrict__ ip, uint32_t port);
int node_destroy(struct Node *node);

#endif /* _NODE_H */