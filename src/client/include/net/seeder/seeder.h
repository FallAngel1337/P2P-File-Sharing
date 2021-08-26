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

#include  <node/node.h>

// Send the specified file from `src` to the socket `fd`
int sendfile(int _fd, struct Node *src);

// Start the seeder service
int seeder_start();

#endif /* _SEEDER_H */