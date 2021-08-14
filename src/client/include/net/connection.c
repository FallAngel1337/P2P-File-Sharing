#include "connection.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>

#define BACKLOG_MAX 5

int connecttos(const char *__restrict__ ip, uint32_t port, 
               const void *__restrict__ _buf, size_t _n)
{
    struct sockaddr_in addr;
    int sockfd;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(inet_addr(ip));
    addr.sin_port = htons(port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0) < 0)) {
        fprintf(stderr, "Was not possible to create the socket: %s\n", strerror(errno));
        return -1;
    }

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf("Was not possible to connect to the server: %s\n", strerror(errno));
        return -1;
    }

    if (send(sockfd, _buf, _n, 0) < 0) {
        fprintf(stderr, "Error sending: %s\n", strerror(errno));
        return -1;
    }

    return 0;

}

int recvfroms(const char *__restrict__ ip, uint32_t port,
              const void *__restrict__ _buf, size_t _n)
{
    return 0;
}

int connecttoc(struct sockaddr_in *addr, const void *__restrict__ _buf, size_t _n)
{
    return 0;
}