/*
 * conn.h - Server connection handle header
 * Author: FallAngel
 * Date: 2021-08-16
 * 
 * Copyright (c) 2021
 */

#ifndef _CONNECTION_H
#define _CONNECTION_H

#include "../../../../include/file_info/file_info.h"

#include <sys/types.h>
#include <stdint.h>
#include <netinet/in.h>

// Recieve the json from client and deserialize
int recvfromc(int _fd, struct file_info *_file_info);

// Send to client the address of the file owner
int sendtoc(int _fd, struct sockaddr_in *_addr);

#endif /* _CONNECTION_H */