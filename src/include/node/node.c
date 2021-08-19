#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <limits.h>
#include <time.h>

struct Node* node_create(const char *__restrict__ ip, uint32_t port)
{
    struct Node *node = malloc(sizeof(struct Node));
    struct file_info *fileinfo = calloc(1, sizeof(struct file_info));
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(ip),
        .sin_port = htons(port),
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