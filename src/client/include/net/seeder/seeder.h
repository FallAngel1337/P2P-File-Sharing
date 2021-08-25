/**
 * seeder.h - Client seeder header
 * Author: FallAngel
 * Date: 2021-08-24
 * 
 * Copyright (c) 2021
 */
/**
 * In this header we'll have some functions to
 * distribute the file among (SUS) other nodes.
*/

#ifndef _SEEDER_H
#define _SEEDER_H

#include  "../../../../include/node/node.h"

// Send the specified file from `src` to `dst`
int sendfile(struct Node *dst, struct Node *src);

#endif /* _SEEDER_H */