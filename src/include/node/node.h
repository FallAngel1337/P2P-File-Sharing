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

#ifndef _NODE_H_
#define _NODE_H_

#include "../file_info/file_info.h"

#include <stdint.h>
#include <netinet/in.h> // struct sockaddr_in

struct Node {
    struct file_info *fileinfo;
    struct sockaddr_in addr;
};

struct Node* node_create(void);
int node_destroy(struct Node*);

#endif /* _NODE_H_ */