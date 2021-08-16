#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <limits.h>
#include <time.h>

static inline uint32_t use_random_port()
{
    srandom(time(NULL));
    return random() % 65536;
}

struct Node* node_create(const char *__restrict__ ip, uint32_t port)
{
    struct Node *node = malloc(sizeof(struct Node));
    struct file_info *fileinfo = calloc(1, sizeof(struct file_info));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(inet_addr(ip)),
        .sin_port = (port) ? htons(port) : htons(use_random_port()),
    };

    file_info_init(fileinfo);
    
    *node = (struct Node) {
        .fileinfo = fileinfo,
        .addr = addr,
    };

    return node;
}

int node_destroy(struct Node *node)
{
    free(node->fileinfo);
    free(node);
    return 0;
}