/**
 * connection.h - Client connection header
 * Author: FallAngel
 * Date: 2021-08-14
 * 
 * Copyright (c) 2021
 */
/**
 * This header contain functions
 * that are used to perform the client's connections
 * and data sending.
 */

#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

// Connect to a node(could be the server too) and send `n` bytes of `buf`.
int connectton(const char *__restrict__ ip, uint32_t port, 
               const void *__restrict__ _buf, size_t _n);

// Recieve from a node and write the reponse in `n` bytes of `_buf`
int recvfromn(int _fd, void *__restrict__ _buf, size_t _n);

#endif /*  _CONNECTION_H */