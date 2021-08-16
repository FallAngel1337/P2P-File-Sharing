/**
 * conn.h - Client connection handle header
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

// Connect to the server and send `n` bytes of `buf`.
int connecttos(const char *__restrict__ ip, uint32_t port,
               const void *__restrict__ _buf, size_t _n);

// Store the response from the server in
// `n` bytes of `buf`. 
int recvfroms(int _fd, void *__restrict__ _buf, size_t _n);

// Connect to another client and send `n` bytes of `buf`.
int connecttoc(struct sockaddr_in *addr, const void *__restrict__ _buf, size_t _n);

#endif /*  _CONNECTION_H */