/*
 * conn.h - Server connection handle header
 * Author: FallAngel
 * Date: 2021-08-16
 * 
 * Copyright (c) 2021
 */

#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <node/node.h>

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

// Recieve the json from client and deserialize
int recvfromc(int _fd, struct Node **_node, uint8_t _flags);

// Send to client the address of the seeder
int sendtoc(int _fd, struct Node *_seeder);

#endif /* _CONNECTION_H */