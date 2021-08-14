/**
 * connection.h - Client connection header
 * Author: FallAngel
 * Date: 2021-08-14
 * 
 * Copyright (c) 2021
 */
/**
 * Thos header contain functions
 * that are used to perform the client's connections
 * and data sending.
 */

#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

// Connect with the server
int connecttos(const char *__restrict__ ip, uint32_t port, 
               const void *__restrict__ _buf, size_t _n);

// Connect to another client
int connecttoc(struct sockaddr_in *addr, const void *__restrict__ _buf, size_t _n);

#endif /*  _CONNECTION_H */